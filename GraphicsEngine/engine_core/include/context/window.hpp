#ifndef ENGINE_CORE_CONTEXT_WINDOW_H
#define ENGINE_CORE_CONTEXT_WINDOW_H

#include <GLFW/glfw3.h>

#include <types.hpp>

#include <memory>
#include <string>

namespace ala::ctx {

// This is class which works with GLFW-provided OpenGL services
class context;

// This is class which works with GLFW-provided monitor API
class monitor;

// This is class which workd with GLFW-provided API related to wotk with native GUI
class window;

class cursor_data {};

class cursor {
  public:
  cursor() = delete;
  cursor(const cursor&) = delete;
  cursor(cursor&&) = delete;

  static std::shared_ptr<cursor> get_custom_cursor(const cursor_data& data);

  static std::shared_ptr<cursor> get_arrow_cursor();
  static std::shared_ptr<cursor> get_crosshair_cursor();
  static std::shared_ptr<cursor> get_hand_cursor();
  static std::shared_ptr<cursor> get_hresize_cursor();
  static std::shared_ptr<cursor> get_vresize_cursor();

  private:
  struct glfw_cursor_deleter {
    void operator()(cursor* cursor);
    void operator()(const cursor* cursor);
  };

  friend class window;  // Window class

  cursor(GLFWcursor* handle);

  mutable GLFWcursor* hcursor_;
};

class cursor_state {};

class window {
  using usize = types::usize;

  public:
  struct size_type {
    int width;
    int height;
  };

  struct maximized_t {
    explicit maximized_t() = default;
  };

  struct resizable_t {
    explicit resizable_t() = default;
  };

  static const maximized_t maximized;
  static const resizable_t resizable;

  window() = delete;
  window(const window& window) = delete;
  window(window&&) = delete;

  window& operator=(const window& monitor) = delete;
  window& operator=(window&& monitor) = delete;

  static std::string default_title();

  static std::shared_ptr<window> create(const std::string& title,
                                        std::shared_ptr<const monitor> parent_monitor);

  static std::shared_ptr<window> create(const std::string& title, const maximized_t& maximized);

  static std::shared_ptr<window> create(const std::string& title, int width, int height);
  static std::shared_ptr<window> create(const std::string& title, int width, int height,
                                        const resizable_t& resizable);

  static std::shared_ptr<window> create(const std::string& title, size_type size);
  static std::shared_ptr<window> create(const std::string& title, size_type size,
                                        const resizable_t& resizable);

  static std::shared_ptr<window> share(std::shared_ptr<const window> parent, const maximized_t& maximized);

  static std::shared_ptr<window> share(std::shared_ptr<const window> parent, const std::string& title,
                                       int width, int height);
  static std::shared_ptr<window> share(std::shared_ptr<const window> parent, const std::string& title,
                                       int width, int height, const resizable_t& resizable);

  static std::shared_ptr<window> share(std::shared_ptr<const window> parent, const std::string& title,
                                       size_type size);
  static std::shared_ptr<window> share(std::shared_ptr<const window> parent, const std::string& title,
                                       size_type size, const resizable_t& resizable);

  void apply_cursor(std::shared_ptr<const cursor> cursor);

  void center_cursor();

  void lock_cursor();
  void unlock_cursor();

  size_type get_size() const;
  size_type get_physical_size() const;

  inline types::usize get_id() const { return id_; }

  private:
  struct glfw_window_deleter {
    void operator()(window* hwnd);
    void operator()(const window* hwnd);
  };

  friend class context;  // OpenGL context

  // This method is related to [event system](./event_queue.hpp)
  friend void create_event_queue(std::shared_ptr<window>);

  window(GLFWwindow* handle, usize id);

  mutable GLFWwindow* hwnd_;

  usize id_;
};

}  // namespace ala::ctx

#endif
