#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void sleep_ms(const uint16_t miliseconds) {
  uint16_t seconds = miliseconds / 1000;

  struct timespec timespec = {
      .tv_sec = seconds,
      .tv_nsec = seconds / 100,
  };

  while (nanosleep(&timespec, NULL) == -1) {
  	perror("nanosleep");
  	break;
  }
}

int is_string_in_array(const char *array[], size_t size, const char *string) {
  for (size_t i = 0; i < size; i++) {
    const char *item = array[i];

    if (!strncmp(string, item, strlen(item))) {
      return 1;
    }
  }

  return 0;
}
