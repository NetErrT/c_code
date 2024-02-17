#include "utils.h"

#include <stdbool.h>
#include <string.h>
#include <time.h>

void *malloc_null(const size_t size) {
  void *pointer = malloc(size);

  if (!pointer) return NULL;

  memset(pointer, 0, size);

  return pointer;
}

int msleep(const unsigned long miliseconds) {
  const struct timespec requested = {
      .tv_sec = miliseconds / 1000,
      .tv_nsec = miliseconds * 1000,
  };

  return nanosleep(&requested, NULL) >= 0;
}

uint8_t percent(const long current, const long max) {
  return ((current * 100) / max);
}
