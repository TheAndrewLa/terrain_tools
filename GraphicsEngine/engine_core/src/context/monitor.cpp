#include <memory>
#include <wnd/monitor.hpp>

#include <cassert>

#include <stdexcept>
#include <string>

namespace ala::wnd {

monitor::monitor(GLFWmonitor* monitor) : hmonitor_(monitor) {}

monitor::ref_t monitor::get_primary() {
  return std::shared_ptr<monitor>(new monitor(glfwGetPrimaryMonitor()));
}

monitor::ref_t monitor::get_by_index(types::int32 index) {
  types::int32 count;
  auto* monitors = glfwGetMonitors(&count);

  assert(monitors != nullptr);
  assert(count > 0);

  if (index >= count) [[unlikely]] {
    throw std::invalid_argument("Invalid index for monitor!");
  }

  return std::make_shared<monitor>(monitors[index]);
}

void monitor::set_gamma(monitor::gamma_t gamma) {
  glfwSetGamma(hmonitor_, gamma);
}

std::string monitor::get_name() const {
  const auto* name = glfwGetMonitorName(hmonitor_);
  assert(name != nullptr);

  return std::string(name);
}

monitor::video_mode monitor::get_video_mode() const {
  const auto* vidmode = glfwGetVideoMode(hmonitor_);
  assert(vidmode != nullptr);

  video_mode mode;

  mode.red_bits = vidmode->redBits;
  mode.green_bits = vidmode->greenBits;
  mode.blue_bits = vidmode->blueBits;

  mode.width = vidmode->width;
  mode.height = vidmode->height;

  mode.refresh_rate = vidmode->refreshRate;

  return mode;
}

GLFWmonitor* monitor::get_handle() {
  return hmonitor_;
}

}  // namespace ala::wnd