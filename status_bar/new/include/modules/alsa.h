#ifndef _MODULES_ALSA_H
#define _MODULES_ALSA_H

#include <alsa/asoundlib.h>

typedef struct {
  snd_mixer_t *mixer;
  snd_mixer_selem_id_t *id;
  snd_mixer_elem_t *elem;
} alsa_t;

typedef struct {
  long volume;
  long max;
  long min;
  int muted;
} alsa_info_t;

alsa_t *alsa_init(const char *name, const int card, const char *device);
alsa_info_t *alsa_info_get(alsa_t *alsa);

int alsa_wait_for_event(alsa_t *alsa);

void alsa_free(alsa_t *alsa);

#endif
