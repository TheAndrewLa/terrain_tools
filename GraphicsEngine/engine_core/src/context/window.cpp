#include <context/monitor.hpp>
#include <context/window.hpp>

#include <cassert>

#include <stdexcept>
#include <string>

namespace ala::ctx {

std::shared_ptr<cursor> cursor::get_custom_cursor(const cursor_data& data) {}

std::shared_ptr<cursor> cursor::get_arrow_cursor() {
  return std::shared_ptr<cursor>(new cursor{glfwCreateStandardCursor(GLFW_ARROW_CURSOR)},
                                 cursor::glfw_cursor_deleter{});
}

std::shared_ptr<cursor> cursor::get_crosshair_cursor() {
  return std::shared_ptr<cursor>(new cursor{glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR)},
                                 cursor::glfw_cursor_deleter{});
}

std::shared_ptr<cursor> cursor::get_hand_cursor() {
  return std::shared_ptr<cursor>(new cursor{glfwCreateStandardCursor(GLFW_HAND_CURSOR)},
                                 cursor::glfw_cursor_deleter{});
}

std::shared_ptr<cursor> cursor::get_hresize_cursor() {
  return std::shared_ptr<cursor>(new cursor{glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR)},
                                 cursor::glfw_cursor_deleter{});
}

std::shared_ptr<cursor> cursor::get_vresize_cursor() {
  return std::shared_ptr<cursor>(new cursor{glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR)},
                                 cursor::glfw_cursor_deleter{});
}

cursor::cursor(GLFWcursor* handle) : hcursor_(handle) {
  if (handle == nullptr) {
    throw std::invalid_argument{"GLFW Window handle should not be null!"};
  }
}

void cursor::glfw_cursor_deleter::operator()(cursor* cursor) {
  glfwDestroyCursor(cursor->hcursor_);
  delete cursor;
}

void cursor::glfw_cursor_deleter::operator()(const cursor* cursor) {
  glfwDestroyCursor(cursor->hcursor_);
  delete cursor;
}

}  // namespace ala::ctx

static ala::types::usize windows_created = 0;

std::string ala::ctx::window::default_title() {
  return "Window #" + std::to_string(windows_created + 1);
}

static inline void glfw_vidmode_hints(std::shared_ptr<const ala::ctx::monitor> monitor) {
  auto vidmode = monitor->get_video_mode();

  glfwWindowHint(GLFW_RED_BITS, vidmode.red_bits);
  glfwWindowHint(GLFW_GREEN_BITS, vidmode.green_bits);
  glfwWindowHint(GLFW_BLUE_BITS, vidmode.blue_bits);
}

static inline void glfw_resizable_hints() {
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

static inline void glfw_maximized_hints() {
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}

static inline void glfw_default_hints() {
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
}

static inline GLFWwindow* glfw_create_window_handle(const char* title, int width, int height,
                                                    GLFWmonitor* monitor, bool show = true) {
  auto* result = glfwCreateWindow(width, height, title, monitor, nullptr);

  if (result == nullptr) {
    throw std::runtime_error{"Failed to create window, try to restart application!"};
  }

  if (show) [[likely]] {
    glfwShowWindow(result);
  }

  windows_created += 1;

  return result;
}

namespace ala::ctx {

std::shared_ptr<window> window::create(const std::string& title,
                                       std::shared_ptr<const monitor> parent_monitor) {
  glfw_vidmode_hints(parent_monitor);
  glfw_default_hints();

  auto* hwnd = glfw_create_window_handle(title.c_str(), 100, 100, parent_monitor->get_handle());
  return std::shared_ptr<window>(new window(hwnd, windows_created - 1), glfw_window_deleter{});
}

std::shared_ptr<window> window::create(const std::string& title, const window::maximized_t& maximized) {
  glfw_vidmode_hints(monitor::get_primary());
  glfw_default_hints();
  glfw_maximized_hints();

  auto* hwnd = glfw_create_window_handle(title.c_str(), 100, 100, nullptr);
  return std::shared_ptr<window>(new window(hwnd, windows_created - 1), glfw_window_deleter{});
}

std::shared_ptr<window> window::create(const std::string& title, int width, int height) {
  glfw_vidmode_hints(monitor::get_primary());
  glfw_default_hints();

  auto* hwnd = glfw_create_window_handle(title.c_str(), width, height, nullptr);
  return std::shared_ptr<window>(new window(hwnd, windows_created - 1), glfw_window_deleter{});
}

std::shared_ptr<window> window::create(const std::string& title, int width, int height,
                                       const window::resizable_t& resizable) {
  glfw_resizable_hints();
  return window::create(title, width, height);
}

std::shared_ptr<window> window::create(const std::string& title, size_type size) {
  return window::create(title, size.width, size.height);
}

std::shared_ptr<window> window::create(const std::string& title, size_type size,
                                       const window::resizable_t& resizable) {
  return window::create(title, size.width, size.height, resizable);
}

window::window(GLFWwindow* handle, types::usize id) : hwnd_(handle), id_(id) {
  if (handle == nullptr) {
    throw std::invalid_argument{"GLFW Window handle should not be null!"};
  }
}

void window::glfw_window_deleter::operator()(window* window) {
  glfwDestroyWindow(window->hwnd_);
  delete window;
}

void window::glfw_window_deleter::operator()(const window* window) {
  glfwDestroyWindow(window->hwnd_);
  delete window;
}

void window::center_cursor() {
  glfwSetCursorPos(hwnd_, 0.5, 0.5);
}

void window::lock_cursor() {
  glfwSetInputMode(hwnd_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void window::unlock_cursor() {
  glfwSetInputMode(hwnd_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

window::size_type window::get_size() const {
  size_type size;
  glfwGetWindowSize(hwnd_, &size.width, &size.height);

  return size;
}

window::size_type window::get_physical_size() const {
  int left, right;
  int bottom, top;

  glfwGetWindowFrameSize(hwnd_, &left, &top, &right, &bottom);

  int width = right - left;
  int height = bottom - top;

  assert(width > 0);
  assert(height > 0);

  return size_type{width, height};
}

}  // namespace ala::ctx
