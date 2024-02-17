#include "modules/date.h"

#include <locale.h>
#include <string.h>
#include <time.h>

#include "utils.h"

char *date_get(const char *format) {
  setlocale(LC_ALL, "");

  time_t timer = time(NULL);
  struct tm info = {0};

  if (!localtime_r(&timer, &info)) return NULL;

  char *buffer = malloc_null(DATE_SIZE_MAX);

  if (!buffer) return NULL;

  strftime(buffer, DATE_SIZE_MAX, format, &info);

  return buffer;
}
