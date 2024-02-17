#ifndef _COMPONENTS_ALSA_H
#define _COMPONENTS_ALSA_H

typedef struct {
	const char *name;
	const int card;
	const char *device;
	const int is_percent;
	const char *format_normal;
	const char *format_muted;
} AlsaOptions;

void *alsa(void *param);

#endif
