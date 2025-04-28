#ifndef ENGINE_CORE_CONTEXT_WINDOW_H
#define ENGINE_CORE_CONTEXT_WINDOW_H

#include <GLFW/glfw3.h>
#include <wnd/monitor.hpp>

#include <types.hpp>

#include <memory>
#include <string>

namespace ala::wnd {

class window {
  public:
  using ref_t = std::shared_ptr<window>;
  using const_ref_t = std::shared_ptr<const window>;

  using title_t = std::string;

  struct size_type {
    types::int32 width;
    types::int32 height;
  };

  window() = delete;
  window(const window& window) = delete;
  window(window&&) = delete;

  window& operator=(const window& monitor) = delete;
  window& operator=(window&& monitor) = delete;

  static ref_t create(const title_t& title, monitor::ref_t parent_monitor);
  static ref_t create(const title_t& title, types::int32 width, types::int32 height);

  void enable_cursor();
  void disable_cursor();

  size_type get_size() const;
  size_type get_frame_size() const;

  private:
  window(GLFWwindow* window_hanle);
  ~window() = default;

  struct ref_deleter_t {
    void operator()(window* hwnd);
    void operator()(const window* hwnd);
  };

  mutable GLFWwindow* hwnd_;
};

}  // namespace ala::wnd

#endif
