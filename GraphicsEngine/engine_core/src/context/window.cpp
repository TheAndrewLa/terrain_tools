#include <context/window.hpp>

#include <cassert>

#include <format>
#include <stdexcept>
#include <string>
#include <vector>

static ala::types::usize windows_created = 0;

static inline void glfw_vidmode_hints(const ala::wnd::monitor::video_mode& vidmode) {
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

std::string&& window::default_title() {
  std::string result = std::format("Default window {}", (windows_created + 1));
  return std::move(result);
}

std::shared_ptr<window> window::create(const std::string& title, std::shared_ptr<monitor> parent_monitor) {
  monitor::video_mode mode = parent_monitor->get_video_mode();

  glfw_vidmode_hints(mode);
  glfw_default_hints();

  auto* hwnd = glfw_create_window_handle(title.c_str(), 100, 100, parent_monitor->get_handle());
  return std::shared_ptr<window>(new window(hwnd, windows_created - 1), glfw_window_deleter{});
}

std::shared_ptr<window> window::create(const std::string& title, const window::maximized_t& maximized) {
  monitor::video_mode mode = monitor::get_primary()->get_video_mode();

  glfw_vidmode_hints(mode);
  glfw_default_hints();
  glfw_maximized_hints();

  auto* hwnd = glfw_create_window_handle(title.c_str(), 100, 100, nullptr);
  return std::shared_ptr<window>(new window(hwnd, windows_created - 1), glfw_window_deleter{});
}

std::shared_ptr<window> window::create(const std::string& title, int width, int height) {
  monitor::video_mode mode = monitor::get_primary()->get_video_mode();

  glfw_vidmode_hints(mode);
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

window::window(GLFWwindow* handle, types::usize id) : hwnd_(handle), id_(id) {}

void window::glfw_window_deleter::operator()(window* hwnd) {
  glfwDestroyWindow(hwnd->hwnd_);
  delete hwnd;
}

void window::glfw_window_deleter::operator()(const window* hwnd) {
  glfwDestroyWindow(hwnd->hwnd_);
  delete hwnd;
}

void window::enable_cursor() {
  glfwSetCursorPos(hwnd_, 0.5, 0.5);
  glfwSetInputMode(hwnd_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void window::disable_cursor() {
  glfwSetCursorPos(hwnd_, 0.5, 0.5);
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

}  // namespace ala::wnd
