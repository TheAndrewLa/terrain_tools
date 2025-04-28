#ifndef ENGINE_CORE_CONTEXT_EVENTS_H
#define ENGINE_CORE_CONTEXT_EVENTS_H

#include <GLFW/glfw3.h>

#include "../wnd/monitor.hpp"
#include "../wnd/window.hpp"

#include "../types.hpp"

#include <concepts>
#include <format>
#include <string>
#include <variant>

namespace ala::context {

template <typename EventT>
concept event = requires(const EventT& event) {
  { event.get_info() } -> std::convertible_to<std::string>;
};

struct monitor_disconnected {
  std::string get_info() const { return std::format("Monitor {} is disconnected!", index_); }

  private:
  types::usize index_;
};

struct monitor_connected {
  std::string get_info() const { return std::format("Monitor {} is connected!", index_); }

  private:
  types::usize index_;
};

using event_type = std::variant<monitor_connected, monitor_disconnected>;

// NOTE: Two types of event queue (both of them are thread-safe)
//  1) Usual event queue - stores all events in one container
//  2) Filtered event queue - filters events by type into several containers

// Usual event queue is faster, Filtered event queue allows to dispatch events of different types in parallel

class event_queue {};

class filtered_event_queue {};

}  // namespace ala::wnd

#endif
