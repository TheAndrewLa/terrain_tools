#ifndef ENGINE_CORE_CONTEXT_EVENTS_H
#define ENGINE_CORE_CONTEXT_EVENTS_H

#include <GLFW/glfw3.h>

#include <context/window.hpp>
#include <functional>
#include <types.hpp>

namespace ala::ctx {

// Window events

// TODO...

// Keyboard events

enum class key_code : int {
  // TODO: Fill up this enum
  w = GLFW_KEY_W,
  a = GLFW_KEY_A,
  s = GLFW_KEY_S,
  d = GLFW_KEY_D,
};

const char8_t* get_key_name(key_code key);

struct key_event {
  key_code code;

  bool shift_modifier;
  bool ctrl_modifier;
  bool alt_modifier;
  bool super_modifier;
};

using key_pressed_event = key_event;
using key_released_event = key_event;

// Mouse events

enum class mouse_button_code : int {
  mouse_button_1 = 0,
  mouse_button_2,
  mouse_button_3,
  mouse_button_4,
  mouse_button_5,
  mouse_button_6,
  mouse_button_7,
  mouse_button_8,
};

struct mouse_button_event {
  mouse_button_code button_code;

  bool shift_modifier;
  bool ctrl_modifier;
  bool alt_modifier;
  bool super_modifier;
};

using mouse_button_pressed_event = mouse_button_event;
using mouse_button_released_event = mouse_button_event;

struct mouse_move_event {
  types::float64 pos_x;
  types::float64 pos_y;
};

void create_event_queue(std::shared_ptr<ctx::window> window);

void poll_events();
void wait_events();
void wait_events(types::float64 timeout);

struct event_dispatcher {
  std::function<void(const key_event&)> key_pressed_dispatcher;
  std::function<void(const key_event&)> key_released_dispatcher;

  std::function<void(const mouse_button_event&)> mouse_btn_pressed_dispatcher;
  std::function<void(const mouse_button_event&)> mouse_btn_released_dispatcher;

  std::function<void(const mouse_move_event&)> mouse_move_dispatcher;
};

void clear_event_queue();

// Event stream object allow to apply object dispatcher

//void iterate_monitor_events(std::function<void(const monitor_event&)> callback);
//void iterate_window_events(std::function<void(const window_event&)> callback);
//void iterate_keyboard_events(std::function<void(const keyboard_event&)> callback);
//void iterate_mouse_events(std::function<void(const mouse_event&)> callback);

}  // namespace ala::ctx

#endif
