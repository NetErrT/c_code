#ifndef _COMPONENTS_KEYBOARD_LAYOUT_H
#define _COMPONENTS_KEYBOARD_LAYOUT_H

#include "modules/keyboard.h"

typedef struct {
	KeyboardLayouts layout;
} KeyboardLayoutOptions;

void *keyboard_layout(void *param);

#endif
