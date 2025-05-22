#include <context/event_system.hpp>

#include <mutex>
#include <thread>

#include <functional>
#include <memory>
#include <type_traits>

// Templated asynchronous event queue
// By design, `event` is a very light object (trivially copyable, movable & destructible)
// Event contains info about event & doesn't own resources

static constexpr auto MAX_QUEUE_LENGTH = static_cast<ala::types::usize>(16);

template <typename T>
class event_queue_container {
  using usize = ala::types::usize;

  public:
  explicit event_queue_container(usize capacity)
      : capacity(capacity), buffer_(std::make_unique<T[]>(capacity)) {}

  event_queue_container(const event_queue_container&) = delete;
  event_queue_container(event_queue_container&&) = default;

  ~event_queue_container() = default;

  event_queue_container& operator=(const event_queue_container&) = delete;
  event_queue_container& operator=(event_queue_container&&) = default;

  void push(const T& event)
    requires(std::is_copy_constructible_v<T>)
  {
    if (full()) {
      throw std::runtime_error{"Too many arguments in event queue!"};
    }

    new (&buffer_[length_++]) T(event);
  }

  void push(T&& event)
    requires(std::is_move_constructible_v<T>)
  {
    if (full()) {
      throw std::runtime_error{"Too many arguments in event queue!"};
    }

    new (&buffer_[length_++]) T(event);
  }

  void clear() {
    if constexpr (std::is_destructible_v<T> && !std::is_trivially_destructible_v<T>) {
      for (usize i = 0; i < length_; i++) {
        buffer_[i].~T();
      }
    }

    length_ = 0;
  }

  // Iterating method

  inline T* begin() { return &buffer_[0]; }

  inline T* end() { return &buffer_[length_]; }

  inline const T* cbegin() const { return &buffer_[0]; }

  inline const T* cend() const { return &buffer_[length_]; }

  // Getting by index

  inline T& operator[](usize index) { return buffer_[index]; }

  inline const T& operator[](usize index) const { return buffer_[index]; }

  inline bool length() const { return length_; }

  private:
  inline bool empty() const { return length_ == 0; }

  inline bool full() const { return length_ == capacity; }

  const usize capacity;

  std::unique_ptr<T[]> buffer_;
  usize length_;
};

template <typename T, ala::types::usize Length>
class event_queue {
  using usize = ala::types::usize;

  public:
  event_queue() = default;

  event_queue(const event_queue& queue) = delete;
  event_queue(event_queue&& queue) = default;

  ~event_queue() = default;

  event_queue& operator=(const event_queue& queue) = delete;
  event_queue& operator=(event_queue&& queue) = default;

  // Push functions

  void push(T&& element) {}

  void push(const T& element) {}

  // Clearing queue, called after dispatching events

  void clear() {
    events_.clear();
    events_reserved_.clear();
  }

  void dispatch_serial(std::function<void(const T&)> func) {
    auto dispatch_lambda = [events_, func]() {
      for (const auto& i : events_) {
        func(i);
      }
    };

    serial_dispatcher_ = std::thread(dispatch_lambda);
  }

  void dispatch_parallel(std::function<void(const T&)> func) {
    auto dispatch_lambda = [events_, func](usize index) {
      func(events_[index]);
    };

    for (usize i = 0; i < events_.length(); i++) {
      parallel_dispatchers_[i] = std::thread(dispatch_lambda, i);
    }
  }

  private:
  event_queue_container<T> events_{Length};
  event_queue_container<T> events_reserved_{Length};

  std::mutex dispatch_lock_;

  std::thread parallel_dispatchers_[Length];
  std::thread serial_dispatcher_;
};

static event_queue<ala::ctx::key_pressed_event, 32> key_pressed_queue;
static event_queue<ala::ctx::key_released_event, 32> key_released_queue;

static event_queue<ala::ctx::mouse_button_pressed_event, 32> mouse_btn_pressed_queue;
static event_queue<ala::ctx::mouse_button_released_event, 32> mouse_btn_released_queue;

static event_queue<ala::ctx::mouse_move_event, 32> mouse_move_queue;

static void key_callback(GLFWwindow* window, int key, int, int action, int mods) {
  ala::ctx::key_event event{};

  event.code = ala::ctx::key_code{key};

  // Special values for modifier bits, see GLFW doc for more info
  enum modifier : int {
    SHIFT = (1 << 0),
    CONTROL = (1 << 1),
    ALT = (1 << 2),
    SUPER = (1 << 3),
  };

  event.shift_modifier = ((mods & modifier::SHIFT) != 0);
  event.ctrl_modifier = ((mods & modifier::CONTROL) != 0);
  event.alt_modifier = ((mods & modifier::ALT) != 0);
  event.super_modifier = ((mods & modifier::SUPER) != 0);

  if (action == GLFW_PRESS) {
    key_pressed_queue.push(event);
  } else if (action == GLFW_RELEASE) {
    key_released_queue.push(event);
  } else [[unlikely]] {
    throw std::runtime_error{"Invalid value of `action` for key callback!"};
  }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  mouse_move_queue.push(ala::ctx::mouse_move_event{xpos, ypos});
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  ala::ctx::mouse_button_event event{};

  event.button_code = ala::ctx::mouse_button_code{button};

  // Special values for modifier bits, see GLFW doc for more info
  enum modifier : int {
    SHIFT = (1 << 0),
    CONTROL = (1 << 1),
    ALT = (1 << 2),
    SUPER = (1 << 3),
  };

  event.shift_modifier = ((mods & modifier::SHIFT) != 0);
  event.ctrl_modifier = ((mods & modifier::CONTROL) != 0);
  event.alt_modifier = ((mods & modifier::ALT) != 0);
  event.super_modifier = ((mods & modifier::SUPER) != 0);

  if (action == GLFW_PRESS) {
    mouse_btn_pressed_queue.push(event);
  } else if (action == GLFW_RELEASE) {
    mouse_btn_released_queue.push(event);
  } else [[unlikely]] {
    throw std::runtime_error{"Invalid value of `action` for key callback!"};
  }
}

namespace ala::ctx {

const char8_t* get_key_name(key_code key) {
  int key_code = static_cast<int>(key);
  return reinterpret_cast<const char8_t*>(glfwGetKeyName(key_code, 0));
}

void create_event_queue(std::shared_ptr<ctx::window> window) {
  glfwSetKeyCallback(window->hwnd_, key_callback);
  glfwSetMouseButtonCallback(window->hwnd_, mouse_button_callback);
  glfwSetCursorPosCallback(window->hwnd_, cursor_position_callback);
}

void poll_events() {
  glfwPollEvents();
}

void wait_events() {
  glfwWaitEvents();
}

void wait_events(types::float64 timeout) {
  glfwWaitEventsTimeout(timeout);
}

void clear_event_queue() {
  key_pressed_queue.clear();
  key_released_queue.clear();

  mouse_btn_pressed_queue.clear();
  mouse_btn_released_queue.clear();

  mouse_move_queue.clear();
}

}  // namespace ala::ctx
