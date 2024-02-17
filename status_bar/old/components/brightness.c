#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "brightness.h"

#define BRIGHTNESS_PATH "/sys/class/backlight/"

void get_file_content(char *buffer, int size, char path[NAME_MAX]) {
  FILE *file = fopen(path, "r");

  if (!file) {
    perror("Failed to open file");
    return;
  }

  if (!fgets(buffer, size, file)) {
    perror("Failed to read file");
    goto clean;
  }

  goto clean;

clean:
  if (file)
    fclose(file);
}

void Brightness(char *buffer, size_t size, const char *driver) {
  char current_brightness[MAX_BRIGHTNESS_LENGTH];
  char max_brightness[MAX_BRIGHTNESS_LENGTH];
  char current_brightness_path[NAME_MAX];
  char max_brightness_path[NAME_MAX];

  snprintf(current_brightness_path, sizeof(current_brightness_path),
           "%s/%s/brightness", BRIGHTNESS_PATH, driver);
  snprintf(max_brightness_path, sizeof(max_brightness_path),
           "%s/%s/max_brightness", BRIGHTNESS_PATH, driver);
  get_file_content(current_brightness, MAX_BRIGHTNESS_LENGTH,
                   current_brightness_path);
  get_file_content(max_brightness, MAX_BRIGHTNESS_LENGTH, max_brightness_path);

  int brightness = (int)((double)atoi(current_brightness) / (double)atoi(max_brightness) * 100);

  snprintf(buffer, size, "%d%%", brightness);
}
