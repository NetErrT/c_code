#include "modules/alsa.h"

#include <pthread.h>

#include "components.h"
#include "components/alsa.h"
#include "utils.h"

void *alsa(void *param) {
  ThreadData *data = (ThreadData *)param;
  AlsaOptions *options = (AlsaOptions *)data->options;

  if (!data || !options) pthread_exit(NULL);

  alsa_t *audio = alsa_init(options->name, options->card, options->device);

  do {
    alsa_info_t *info = alsa_info_get(audio);

    if (!info) break;

    long volume =
        options->is_percent ? percent(info->volume, info->max) : info->volume;

    snprintf(data->buffer, sizeof(data->buffer),
             info->muted ? options->format_muted : options->format_normal,
             volume);
    update();

    free(info);
  } while (alsa_wait_for_event(audio));

  pthread_exit(NULL);
}
