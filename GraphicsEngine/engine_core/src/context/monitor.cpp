#include <context/monitor.hpp>

#include <cassert>

#include <stdexcept>

namespace ala::ctx {

monitor::monitor(GLFWmonitor* monitor) {
  if (monitor == nullptr) {
    throw std::invalid_argument{"Monitor handle is NULL!"};
  }

  hmonitor_ = monitor;
}

std::shared_ptr<monitor> monitor::get_primary() {
  return std::shared_ptr<monitor>(new monitor(glfwGetPrimaryMonitor()));
}

void monitor::set_gamma(float gamma) {
  glfwSetGamma(hmonitor_, gamma);
}

void monitor::set_gamma(const monitor::gamma_ramp& gamma) {
  GLFWgammaramp ramp;

  ramp.size = 256;
  ramp.red = const_cast<unsigned short*>(std::addressof(gamma.red[0]));
  ramp.green = const_cast<unsigned short*>(std::addressof(gamma.green[0]));
  ramp.blue = const_cast<unsigned short*>(std::addressof(gamma.blue[0]));

  glfwSetGammaRamp(hmonitor_, &ramp);
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

monitor::size_type monitor::get_physical_size() const {
  size_type size;
  glfwGetMonitorPhysicalSize(hmonitor_, &size.width, &size.height);

  return size;
}

GLFWmonitor* monitor::get_handle() const {
  return hmonitor_;
}

}  // namespace ala::wnd