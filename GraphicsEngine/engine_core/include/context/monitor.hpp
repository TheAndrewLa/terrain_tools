#ifndef ENGINE_CORE_CONTEXT_MONITOR_H
#define ENGINE_CORE_CONTEXT_MONITOR_H

#include <GLFW/glfw3.h>

#include <types.hpp>

#include <array>
#include <memory>
#include <string>

namespace ala::ctx {

class window;

class monitor {
  public:
  struct gamma_ramp {
    std::array<unsigned short, 256> red;
    std::array<unsigned short, 256> green;
    std::array<unsigned short, 256> blue;
  };

  struct video_mode {
    int width, height;
    int red_bits, green_bits, blue_bits;
    int refresh_rate;
  };

  struct size_type {
    int width;
    int height;
  };

  monitor() = delete;
  monitor(const monitor& monitor) = delete;
  monitor(monitor&& monitor) = delete;

  monitor& operator=(const monitor& monitor) = delete;
  monitor& operator=(monitor&& monitor) = delete;

  static std::shared_ptr<monitor> get_primary();

  std::string get_name() const;
  video_mode get_video_mode() const;
  size_type get_physical_size() const;

  void set_gamma(float gamma);
  void set_gamma(const gamma_ramp& gamma);

  private:
  friend class window;

  monitor(GLFWmonitor* monitor_handle);

  GLFWmonitor* get_handle();

  mutable GLFWmonitor* hmonitor_;
};

}  // namespace ala::wnd

#endif
