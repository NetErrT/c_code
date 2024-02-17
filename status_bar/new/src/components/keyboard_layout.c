#include "components/keyboard_layout.h"

#include <pthread.h>
#include <string.h>

#include "components.h"

void *keyboard_layout(void *param) {
  ThreadData *data = (ThreadData *)param;
  KeyboardLayoutOptions *options = (KeyboardLayoutOptions *)data->options;

  if (!data || !options) pthread_exit(NULL);

  keyboard_t *keyboard = keyboard_init();

  do {
    char *layout = keyboard_get_layout(keyboard, options->layout);

    if (!layout) break;

    strncpy(data->buffer, layout, sizeof(data->buffer));
    update();

    free(layout);
  } while (keyboard_wait_for_event(keyboard, SwitchLayout));

  pthread_exit(NULL);
}
