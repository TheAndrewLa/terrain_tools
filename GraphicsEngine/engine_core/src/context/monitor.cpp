#include <context/monitor.hpp>

#include <cassert>

#include <stdexcept>

namespace ala::wnd {

monitor::monitor(GLFWmonitor* monitor) : hmonitor_(monitor) {}

shared_ptr<monitor> monitor::get_primary() {
  return std::shared_ptr<monitor>(new monitor(glfwGetPrimaryMonitor()));
}

shared_ptr<monitor> monitor::get_by_index(types::int32 index) {
  types::int32 count;
  auto* monitors = glfwGetMonitors(&count);

  assert(monitors != nullptr);
  assert(count > 0);

  if (index >= count) {
    throw std::invalid_argument("Invalid index for monitor!");
  }

  return shared_ptr<monitor>(new monitor(monitors[index]));
}

types::usize monitor::get_count() {
  types::int32 count;
  glfwGetMonitors(&count);

  if (count <= 0) {
    throw std::runtime_error("Invalid number of monitors!");
  }

  return static_cast<types::usize>(count);
}

void monitor::set_gamma(monitor::gamma gamma) {
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

monitor::size_type monitor::get_physical_size() const {
  size_type size;
  glfwGetMonitorPhysicalSize(hmonitor_, &size.width, &size.height);

  return size;
}

GLFWmonitor* monitor::get_handle() {
  return hmonitor_;
}

}  // namespace ala::wnd