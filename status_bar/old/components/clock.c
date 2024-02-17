#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "clock.h"

void Clock(char *buffer, const size_t size, const char *format) {
  time_t timer;
  struct tm info;

	memset(buffer, 0, size);
  time(&timer);

  if (!localtime_r(&timer, &info)) {
    perror("localtime_r");

    return;
  }

  strftime(buffer, size, format, &info);
}
