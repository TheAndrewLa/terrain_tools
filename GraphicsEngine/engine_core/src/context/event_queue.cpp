#include <context/event_queue.hpp>
#include <type_traits>

// Templated event queue
// By design, `event` is a very light object (trivially copyable, movable & destructible)

#define MAX_QUEUE_LENGTH ((::ala::types::usize)16)

template <typename T, ala::types::usize MaxLength = MAX_QUEUE_LENGTH>
  requires(std::is_trivially_copy_constructible_v<T> && std::is_trivially_move_constructible_v<T> &&
           std::is_trivially_destructible_v<T>)
class event_queue {
  public:
  event_queue() : length_(0), elements_(::new T[MaxLength]) {}

  event_queue(const event_queue& queue) = delete;

  event_queue(event_queue&& queue) {
    elements_ = queue.elements_;
    length_ = queue.length_;

    queue.elements_ = nullptr;
  }

  ~event_queue() { ::delete[] (elements_); }

  event_queue& operator=(const event_queue& queue) = delete;

  event_queue& operator=(event_queue&& queue) {
    ::delete[] (elements_);

    elements_ = queue.elements_;
    length_ = queue.length_;

    queue.elements_ = nullptr;
  }

  // Push n' Pop

  void push(T&& element) { ::new (&elements_[length_]) T(element); }

  void push(const T& element) { ::new (&elements_[length_]) T(element); }

  void pop() { length -= 1; }

  T& peek() { return elements_[length - 1]; }

  const T& peek() const { return elements_[length - 1]; }

  // Iterators

  T* begin() { return &elements_[0]; }

  T* end() { return &elements_[length_]; }

  const T* cbegin() const { return &elements_[0]; }

  const T* cend() const { return &elements_[length_]; }

  // Clearing function, doesn't call destructor because of trivially destructible type

  void clear() { length_ = 0; }

  private:
  T* elements_;
  ala::types::usize length_;
};

// Different arrays as a queue for events

static event_queue<ala::ctx::key_pressed_event> key_pressed_queue;
static event_queue<ala::ctx::key_released_event> key_released_queue;

static event_queue<ala::ctx::mouse_button_pressed_event> mouse_btn_pressed_queue;
static event_queue<ala::ctx::mouse_button_released_event> mouse_btn_released_queue;

static event_queue<ala::ctx::mouse_move_event> mouse_move_queue;

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
  mouse_move_queue.push({.pos_x = xpos, .pos_y = ypos});
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}

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
