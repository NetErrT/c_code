#include "modules/keyboard.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// include this after stdio!!
#include <X11/extensions/XKBrules.h>

#include "utils.h"

static uint8_t get_state_group(keyboard_t *keyboard);
static void xkb_vars_free(XkbRF_VarDefsRec *xkb_vars);

keyboard_t *keyboard_init(void) {
  keyboard_t *keyboard = malloc_null(sizeof(keyboard_t));

  if (!keyboard) return NULL;

  keyboard->display = XOpenDisplay(NULL);

  if (!keyboard->display) {
    keyboard_free(keyboard);
    return NULL;
  }

  keyboard->desc = XkbAllocKeyboard();

  if (!keyboard->desc) {
    keyboard_free(keyboard);
    return NULL;
  }

  return keyboard;
}

static char *keyboard_get_layout_country(keyboard_t *keyboard) {
  if (!keyboard) return NULL;

  const uint8_t group = get_state_group(keyboard);
  const char *delimeter = ",";

  XkbRF_VarDefsRec xkb_vars = {0};
  XkbRF_GetNamesProp(keyboard->display, NULL, &xkb_vars);

  char *layout_save = NULL;
  char *layout = strtok_r(xkb_vars.layout, delimeter, &layout_save);

  for (uint8_t i = 0; i < group; i++) {
    layout = strtok_r(NULL, delimeter, &layout_save);

    if (!layout) return NULL;
  }

  layout = strdup(layout);

  xkb_vars_free(&xkb_vars);

  return layout;
}

static char *keyboard_get_layout_full(keyboard_t *keyboard) {
  if (!keyboard) return NULL;

  const uint8_t group = get_state_group(keyboard);

  XkbGetNames(keyboard->display, XkbGroupNamesMask, keyboard->desc);

  return XGetAtomName(keyboard->display, keyboard->desc->names->groups[group]);
}

char *keyboard_get_layout(keyboard_t *keyboard, KeyboardLayouts layout) {
  switch (layout) {
    case Full:
      return keyboard_get_layout_full(keyboard);
    case Country:
      return keyboard_get_layout_country(keyboard);
    default:
      return NULL;
  }
}

int keyboard_wait_for_event(keyboard_t *keyboard, KeyboardEvents events) {
  if (!keyboard) return false;

  Display *display = keyboard->display;

  const unsigned int device = XkbUseCoreKbd;
  const unsigned int type = XkbStateNotify;
  const unsigned long type_mask = XkbAllStateComponentsMask;

  if (!XkbSelectEventDetails(display, device, type, type_mask, events)) {
    return false;
  }

  XEvent event = {0};

  XNextEvent(display, &event);

  return true;
}

int keyboard_get_indicator(keyboard_t *keyboard, KeyboardIndicators indicator) {
  XKeyboardState state = {0};

  XGetKeyboardControl(keyboard->display, &state);

  const unsigned long mask = state.led_mask;
  const unsigned int indicator_bit = 1 << indicator;

  return (mask & indicator_bit) > 0;
}

void keyboard_free(keyboard_t *keyboard) {
  if (!keyboard) return;
  if (keyboard->display) XCloseDisplay(keyboard->display);
  if (keyboard->desc) XkbFreeKeyboard(keyboard->desc, 0, 1);

  free(keyboard);
}

static uint8_t get_state_group(keyboard_t *keyboard) {
  XkbStateRec state = {0};

  XkbGetState(keyboard->display, XkbUseCoreKbd, &state);

  return state.group;
}

static void xkb_vars_free(XkbRF_VarDefsRec *xkb_vars) {
  if (!xkb_vars) return;

  free(xkb_vars->extra_names);
  free(xkb_vars->layout);
  free(xkb_vars->model);
  free(xkb_vars->options);
  free(xkb_vars->variant);
}
