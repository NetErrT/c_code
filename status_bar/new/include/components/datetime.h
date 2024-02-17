#ifndef _COMPONENTS_DATETIME_H
#define _COMPONENTS_DATETIME_H

typedef struct {
  const char *format;
  const long timeout;
} DatetimeOptions;

void *datetime(void *param);

#endif
