#ifndef ENGINE_CORE_CONTEXT_WINDOW_H
#define ENGINE_CORE_CONTEXT_WINDOW_H

#include <GLFW/glfw3.h>

#include <context/monitor.hpp>
#include <types.hpp>

#include <memory>
#include <string>

namespace ala::ctx {

class window {
  public:
  struct size_type {
    int width;
    int height;
  };

  struct maximized_t {};

  struct resizable_t {};

  static const maximized_t maximized;
  static const resizable_t resizable;

  window() = delete;
  window(const window& window) = delete;
  window(window&&) = delete;

  window& operator=(const window& monitor) = delete;
  window& operator=(window&& monitor) = delete;

  static std::string&& default_title();

  static std::shared_ptr<window> create(const std::string& title, std::shared_ptr<monitor> parent_monitor);

  static std::shared_ptr<window> create(const std::string& title, const maximized_t& maximized);

  static std::shared_ptr<window> create(const std::string& title, int width, int height);
  static std::shared_ptr<window> create(const std::string& title, int width, int height,
                                        const resizable_t& resizable);

  static std::shared_ptr<window> create(const std::string& title, size_type size);
  static std::shared_ptr<window> create(const std::string& title, size_type size,
                                        const resizable_t& resizable);

  // TODO: advanced cursor management
  void enable_cursor();
  void disable_cursor();

  size_type get_size() const;
  size_type get_physical_size() const;

  inline types::usize get_id() const { return id_; }

  private:
  window(GLFWwindow* handle, types::usize id);

  struct glfw_window_deleter {
    void operator()(window* hwnd);
    void operator()(const window* hwnd);
  };

  friend void create_event_queue(std::shared_ptr<window>);

  mutable GLFWwindow* hwnd_;
  types::usize id_;
};

}  // namespace ala::ctx

#endif
