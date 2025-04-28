#ifndef ENGINE_CORE_CONTEXT_MONITOR_H
#define ENGINE_CORE_CONTEXT_MONITOR_H

#include <GLFW/glfw3.h>

#include "../types.hpp"

#include <memory>
#include <string>

namespace ala::wnd {

class monitor {
  public:
  using ref_t = std::shared_ptr<monitor>;
  using const_ref_t = std::shared_ptr<const monitor>;

  using gamma_t = types::float32;

  struct video_mode {
    types::int32 width, height;
    types::int32 red_bits, green_bits, blue_bits;
    types::int32 refresh_rate;
  };

  monitor() = delete;
  monitor(const monitor& monitor) = delete;
  monitor(monitor&& monitor) = delete;

  monitor& operator=(const monitor& monitor) = delete;
  monitor& operator=(monitor&& monitor) = delete;

  static ref_t get_primary();
  static ref_t get_by_index(types::int32 index);

  std::string get_name() const;
  video_mode get_video_mode() const;

  void set_gamma(gamma_t gamma);

  GLFWmonitor* get_handle();

  private:
  monitor(GLFWmonitor* monitor_handle);

  mutable GLFWmonitor* hmonitor_;
};

}  // namespace ala::wnd

#endif
