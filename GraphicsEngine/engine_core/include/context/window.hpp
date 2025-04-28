#ifndef ENGINE_CORE_CONTEXT_WINDOW_H
#define ENGINE_CORE_CONTEXT_WINDOW_H

#include <GLFW/glfw3.h>

#include <context/monitor.hpp>

#include <types.hpp>

#include <memory>
#include <string>

using std::shared_ptr;

namespace ala::wnd {

class window {
  public:
  struct size_type {
    types::int32 width;
    types::int32 height;
  };

  window() = delete;
  window(const window& window) = delete;
  window(window&&) = delete;

  window& operator=(const window& monitor) = delete;
  window& operator=(window&& monitor) = delete;

  static shared_ptr<window> create(const std::string& title, shared_ptr<monitor> parent_monitor);
  static shared_ptr<window> create(const std::string& title, types::int32 width, types::int32 height);

  void enable_cursor();
  void disable_cursor();

  size_type get_size() const;
  size_type get_frame_size() const;

  private:
  window(GLFWwindow* window_hanle);
  ~window() = default;

  struct glfw_window_deleter {
    void operator()(window* hwnd);
    void operator()(const window* hwnd);
  };

  mutable GLFWwindow* hwnd_;
};

}  // namespace ala::wnd

#endif
