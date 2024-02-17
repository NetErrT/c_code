#include "components/keyboard_indicator.h"

#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "components.h"

void *keyboard_indicator(void *param) {
  ThreadData *data = (ThreadData *)param;

  const char *format = data->options;

  if (!format) pthread_exit(NULL);

  char buffer[128] = {0};
  keyboard_t *keyboard = keyboard_init();

  const char format_caps = 'c';
  const char format_numlock = 'n';

  do {
    memset(buffer, 0, sizeof(buffer));

    const int capslock = keyboard_get_indicator(keyboard, CapsLock);
    const int numlock = keyboard_get_indicator(keyboard, NumLock);

    for (int i = 0, n = 0; i < (int)strlen(format); i++) {
      if (format[i] != '?') {
        buffer[n++] = format[i];
        continue;
      }

      if (format[i + 1] == 'c') {
        buffer[n++] = capslock ? toupper(format_caps) : format_caps;
      } else if (format[i + 1] == 'n') {
        buffer[n++] = numlock ? toupper(format_numlock) : format_numlock;
      } else {
        buffer[n++] = format[i];
        continue;
      }

      i += 1;
    }

    strncpy(data->buffer, buffer, sizeof(data->buffer));
    update();
  } while (keyboard_wait_for_event(keyboard, ChangeIndicator));

  pthread_exit(NULL);
}
