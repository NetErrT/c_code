#include "components/datetime.h"

#include <pthread.h>
#include <string.h>

#include "components.h"
#include "modules/date.h"
#include "utils.h"

void *datetime(void *param) {
  ThreadData *data = (ThreadData *)param;
  DatetimeOptions *options = (DatetimeOptions *)data->options;

  if (!data || !options) pthread_exit(NULL);

  do {
    char *date = date_get(options->format);

    if (!data) break;

    strncpy(data->buffer, date, sizeof(data->buffer) - 1);
    update();

    free(date);
  } while (msleep(options->timeout));

  pthread_exit(NULL);
}
