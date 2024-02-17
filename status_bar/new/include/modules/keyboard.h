#ifndef _MODULES_XKEYBOARD_H
#define _MODULES_XKEYBOARD_H

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  Display *display;
  XkbDescRec *desc;
} keyboard_t;

typedef enum {
  CapsLock = 0,
  NumLock = 1,
} KeyboardIndicators;

typedef enum {
	Country,
	Full,
} KeyboardLayouts;

typedef enum {
  SwitchLayout = XkbGroupStateMask,
  ChangeIndicator = XkbModifierLockMask,
} KeyboardEvents;

keyboard_t *keyboard_init(void);

char *keyboard_get_layout(keyboard_t *keyboard, KeyboardLayouts layout);

int keyboard_wait_for_event(keyboard_t *keyboard, KeyboardEvents events);
int keyboard_get_indicator(keyboard_t *keyboard, KeyboardIndicators indicator);

void keyboard_free(keyboard_t *keyboard);

#endif
