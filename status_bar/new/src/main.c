#include <X11/Xlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "components.h"
#include "config.h"

#define LENGTH(array) sizeof(array) / sizeof(array[0])

static ThreadData data[LENGTH(args)];
static pthread_t threads[LENGTH(args)];

void update(void) {
  char buffer[1024] = {0};

  for (size_t i = 0, len = 0; i < LENGTH(args); i++) {
    strncpy(buffer + len, data[i].buffer, sizeof(buffer) - len);
    len += strlen(data[i].buffer);
  }

  Display *display = XOpenDisplay(NULL);
  Window window = XDefaultRootWindow(display);

  XSync(display, 0);
  XStoreName(display, window, buffer);
  XFlush(display);

  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

int main(void) {
  const size_t size = LENGTH(args);

  for (size_t i = 0; i < size; i++) {
    data[i].options = args[i].param;

    pthread_create(&threads[i], NULL, args[i].component, &data[i]);
  }

  for (size_t i = 0; i < size; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
