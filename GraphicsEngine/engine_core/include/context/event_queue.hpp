#ifndef ENGINE_CORE_CONTEXT_EVENTS_H
#define ENGINE_CORE_CONTEXT_EVENTS_H

#include <GLFW/glfw3.h>

#include <types.hpp>

#include <context/monitor.hpp>
#include <context/window.hpp>

#include <functional>
#include <utility>
#include <variant>

namespace ala::context {

// Monitor & Window events

struct monitor_disconnected {
  types::usize monitor_index;
};

struct monitor_connected {
  types::usize monitor_index;
};

// Keyboard events

struct key_pressed_event {
  types::uint32 scan_code;
  types::uint8 modifier_bit;
};

struct key_released_event {
  types::uint32 scan_code;
  types::uint8 modifier_bit;
};

struct key_repeated_event {
  types::uint32 scan_code;
  types::uint8 modifier_bit;
};

// Mouse events

struct mouse_button_pressed_event {
  types::int32 button_code;
  types::uint8 modifier_bit;
};

struct mouse_button_released_event {
  types::int32 button_code;
  types::uint8 modifier_bit;
};

struct mouse_move_event {
  types::float64 pos_x;
  types::float64 pos_y;
};

struct mouse_scroll_event {
  types::float64 x_offset;
  types::float64 y_offset;
};

// Variant types for different event types

using monitor_event = std::variant<monitor_connected, monitor_disconnected>;

using keyboard_event = std::variant<key_pressed_event, key_released_event, key_repeated_event>;

using mouse_event = std::variant<mouse_button_pressed_event, mouse_button_released_event, mouse_move_event,
                                 mouse_scroll_event>;

// IDEA: event queue

// Event system is a bunch of objects in static memory (queues, mutexes, rwlock, etc)

// Events are divied into categories:
// 1) Window & monitor events
// 2) Keyboard events
// 3) Mouse events

void initialize_event_queue();
void release_event_queue();

void wait_for_events();
void wait_for_events(types::float64 timeout);

void iterate_monitor_events(std::function<void(const monitor_event&)> callback);
void iterate_keyboard_events(std::function<void(const keyboard_event&)> callback);
void iterate_mouse_events(std::function<void(const mouse_event&)> callback);

}  // namespace ala::context

#endif
