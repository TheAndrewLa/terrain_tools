#include <context/window.hpp>

#include <cassert>

#include <stdexcept>
#include <string>

namespace ala::wnd {

window::window(GLFWwindow* window) : hwnd_(window) {}

void window::glfw_window_deleter::operator()(window* hwnd) {
  glfwDestroyWindow(hwnd->hwnd_);
  delete hwnd;
}

void window::glfw_window_deleter::operator()(const window* hwnd) {
  glfwDestroyWindow(hwnd->hwnd_);
  delete hwnd;
}

shared_ptr<window> window::create(const std::string& title, shared_ptr<monitor> parent_monitor) {
  monitor::video_mode mode = parent_monitor->get_video_mode();

  glfwWindowHint(GLFW_RED_BITS, mode.red_bits);
  glfwWindowHint(GLFW_GREEN_BITS, mode.green_bits);
  glfwWindowHint(GLFW_BLUE_BITS, mode.blue_bits);

  auto* hwnd = glfwCreateWindow(100, 100, title.c_str(), nullptr, nullptr);
  assert(hwnd != nullptr);

  glfwSetWindowMonitor(hwnd, parent_monitor->get_handle(), 0, 0, mode.width, mode.height, mode.refresh_rate);

  glfwShowWindow(hwnd);

  return shared_ptr<window>(new window(hwnd), glfw_window_deleter{});
}

shared_ptr<window> window::create(const std::string& title, types::int32 width, types::int32 height) {
  monitor::video_mode mode = monitor::get_primary()->get_video_mode();

  glfwWindowHint(GLFW_RED_BITS, mode.red_bits);
  glfwWindowHint(GLFW_GREEN_BITS, mode.green_bits);
  glfwWindowHint(GLFW_BLUE_BITS, mode.blue_bits);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

  auto* hwnd = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  assert(hwnd != nullptr);

  glfwShowWindow(hwnd);

  return shared_ptr<window>(new window(hwnd), glfw_window_deleter{});
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

window::size_type window::get_frame_size() const {
  types::int32 left, right;
  types::int32 bottom, top;

  glfwGetWindowFrameSize(hwnd_, &left, &top, &right, &bottom);

  auto width = right - left;
  auto height = bottom - top;

  assert(width > 0);
  assert(height > 0);

  return size_type(width, height);
}

}  // namespace ala::wnd
