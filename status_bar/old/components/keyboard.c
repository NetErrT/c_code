#include <X11/XKBlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../utils.h"
#include "keyboard.h"

char *get_layout_from_symbols(char *symbols, const uint8_t group) {
  const char *delimeter = "+:";
  const char *invalids[] = {"evdev", "inet", "pc", "base"};
  char *token_save = NULL;
  char *token = strtok_r(symbols, delimeter, &token_save);

  uint8_t count = 0;

  while (token != NULL) {
    if (!is_string_in_array(invalids, ARRAY_LENGTH(invalids), token)) {
      count++;
    }

    if (count - 1 == group) {
      return strdup(token);
    }

    token = strtok_r(NULL, delimeter, &token_save);
  }

  return NULL;
}

void Keyboard(char *buffer, size_t size) {
  XkbStateRec state;
  Display *display = NULL;
  XkbDescPtr keyboard = NULL;
  char *symbols = NULL;
  char *layout = NULL;

  memset(buffer, 0, size);

  display = XOpenDisplay(NULL);

  if (!display) {
    perror("XOpenDisplay: Failed to open display");
    goto clean;
  }

  keyboard = XkbGetKeyboard(display, XkbSymbolsNameMask, XkbUseCoreKbd);

  if (!keyboard) {
    perror("XkbAllocKeyboard: Failed to allocate keyboard");
    goto clean;
  }

  if (XkbGetNames(display, XkbSymbolsNameMask, keyboard)) {
    perror("XkbGetNames: Failed to retrieve key symbols");
    goto clean;
  }

  if (XkbGetState(display, XkbUseCoreKbd, &state)) {
    perror("XkbGetState: Failed to retrieve keyboard state");
    goto clean;
  }

  symbols = XGetAtomName(display, keyboard->names->symbols);

  if (!symbols) {
    perror("XGetAtomName: Failed to get atom name");
    goto clean;
  }

  layout = get_layout_from_symbols(symbols, state.group);

  if (!layout) {
    perror("Current layout is null");
    goto clean;
  }

  strncpy(buffer, layout, size);
  free(layout);

  goto clean;

clean:
  if (display)
    XCloseDisplay(display);
  if (keyboard)
    XkbFreeKeyboard(keyboard, XkbSymbolsNameMask, XkbUseCoreKbd);
  if (symbols)
    XFree(symbols);
}
