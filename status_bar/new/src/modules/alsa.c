#include "modules/alsa.h"

#include <stdbool.h>

#include "utils.h"

alsa_t *alsa_init(const char *restrict name, const int card,
                  const char *restrict device) {
  alsa_t *alsa = malloc_null(sizeof(alsa_t));

  if (!alsa) {
    return NULL;
  }

  if (snd_mixer_open(&alsa->mixer, 0)) {
    alsa_free(alsa);
    return NULL;
  }

  if (snd_mixer_attach(alsa->mixer, name)) {
    alsa_free(alsa);
    return NULL;
  }

  if (snd_mixer_selem_register(alsa->mixer, NULL, NULL)) {
    alsa_free(alsa);
    return NULL;
  }

  if (snd_mixer_load(alsa->mixer)) {
    alsa_free(alsa);
    return NULL;
  }

  snd_mixer_selem_id_malloc(&alsa->id);
  snd_mixer_selem_id_set_name(alsa->id, device);
  snd_mixer_selem_id_set_index(alsa->id, card);

  alsa->elem = snd_mixer_find_selem(alsa->mixer, alsa->id);

  if (!alsa->elem) {
    alsa_free(alsa);
    return NULL;
  }

  return alsa;
}

alsa_info_t *alsa_info_get(alsa_t *alsa) {
  alsa_info_t *info = malloc_null(sizeof(alsa_info_t));

  if (!info) return NULL;

  snd_mixer_elem_t *elem = alsa->elem;
  const snd_mixer_selem_channel_id_t channel = SND_MIXER_SCHN_MONO;

  snd_mixer_selem_get_playback_volume_range(elem, &info->min, &info->max);

  if (snd_mixer_selem_get_playback_volume(elem, channel, &info->volume)) {
    free(info);
    return NULL;
  }

  if (snd_mixer_selem_get_playback_switch(elem, channel, &info->muted)) {
    free(info);
    return NULL;
  };

  info->muted = !info->muted;

  return info;
}

int alsa_wait_for_event(alsa_t *alsa) {
  if (!alsa) return false;

  int status = snd_mixer_wait(alsa->mixer, -1);

  if (status < 0) return false;

  status = snd_mixer_handle_events(alsa->mixer);

  return true;
}

void alsa_free(alsa_t *alsa) {
  if (!alsa) return;
  if (alsa->id) snd_mixer_selem_id_free(alsa->id);
  if (alsa->mixer) snd_mixer_close(alsa->mixer);

  free(alsa);
}
