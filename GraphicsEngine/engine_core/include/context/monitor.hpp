#ifndef ENGINE_CORE_CONTEXT_MONITOR_H
#define ENGINE_CORE_CONTEXT_MONITOR_H

#include <GLFW/glfw3.h>

#include <types.hpp>

#include <memory>
#include <string>

using std::shared_ptr;

namespace ala::wnd {

class window;

class monitor {
  public:
  using gamma = types::float32;

  struct video_mode {
    types::int32 width, height;
    types::int32 red_bits, green_bits, blue_bits;
    types::int32 refresh_rate;
  };

  struct size_type {
    types::int32 width;
    types::int32 height;
  };

  monitor() = delete;
  monitor(const monitor& monitor) = delete;
  monitor(monitor&& monitor) = delete;

  monitor& operator=(const monitor& monitor) = delete;
  monitor& operator=(monitor&& monitor) = delete;

  static shared_ptr<monitor> get_primary();

  static types::usize get_count();
  static shared_ptr<monitor> get_by_index(types::int32 index);

  std::string get_name() const;
  video_mode get_video_mode() const;
  size_type get_physical_size() const;

  void set_gamma(gamma gamma);

  private:
  friend class window;

  monitor(GLFWmonitor* monitor_handle);

  GLFWmonitor* get_handle();

  mutable GLFWmonitor* hmonitor_;
};

}  // namespace ala::wnd

#endif
