#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <math.h>

#include "audio.h"

void Audio(char *buffer, const size_t size, const char *device, const int card,
           const char *control) {
  long min, max, volume;
  snd_mixer_t *mixer = NULL;
  snd_mixer_elem_t *elem = NULL;
  snd_mixer_selem_id_t *id = NULL;
  int is_mute = 0;

  memset(buffer, 0, size);

  if (snd_mixer_open(&mixer, 0)) {
    perror("Failed to open mixer");
    return;
  }

  if (snd_mixer_attach(mixer, device)) {
    perror("Failed to attach device");
    goto clean;
  }

  if (snd_mixer_selem_register(mixer, NULL, NULL)) {
    perror("Failed to register element");
    goto clean;
  }

  if (snd_mixer_load(mixer)) {
    perror("Failed to load mixer");
    goto clean;
  }

  snd_mixer_selem_id_malloc(&id);
  snd_mixer_selem_id_set_name(id, control);
  snd_mixer_selem_id_set_index(id, card);

  elem = snd_mixer_find_selem(mixer, id);

  if (!elem) {
    perror("Failed to find element");
    goto clean;
  }

  if (strcmp(control, "Master") == 0) {
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

    if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO,
                                            &volume)) {
      perror("Failed to get volume");
      goto clean;
    }

    if (snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_MONO,
                                            &is_mute)) {
      perror("Failed to get mute state");
      goto clean;
    }
  } else {
		snd_mixer_selem_get_capture_volume_range(elem, &min, &max);

		if (snd_mixer_selem_get_capture_volume(elem, SND_MIXER_SCHN_MONO, &volume)) {
			perror("Failed to get capture volume");
			goto clean;
		}

		snd_mixer_selem_get_capture_switch(elem, SND_MIXER_SCHN_MONO, &is_mute);
	}

  const int volume_percent = (int)rint((double)volume / (double)max * 100);
  const char *is_mute_string = is_mute == 0 ? "(muted)" : "";

  snprintf(buffer, size, "%d%%%s", volume_percent, is_mute_string);
clean:
  if (mixer)
    snd_mixer_close(mixer);
  if (id)
    snd_mixer_selem_id_free(id);

	snd_config_update_free_global();
}
