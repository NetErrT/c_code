#include "X11/Xlib.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "components/audio.h"
#include "components/brightness.h"
#include "components/clock.h"
#include "components/keyboard.h"
#include "utils.h"

#define MAX_CLOCK_LEN 20
#define MAX_KEYBOARD_LEN 3
#define MAX_MAIN_VOLUME_LEN 12
#define MAX_MICRO_VOLUME_LEN 12

struct Data {
  char clock[MAX_CLOCK_LEN];
  char keyboard[MAX_KEYBOARD_LEN];
  char brightness[MAX_BRIGHTNESS_LENGTH];
  struct Volume {
    char main[MAX_MAIN_VOLUME_LEN];
    char micro[MAX_MICRO_VOLUME_LEN];
  } volume;
};

void update_data(struct Data *data) {
  Display *display = XOpenDisplay(NULL);
  char status[1024] = {0};

  if (!display) {
    perror("Failed to open display");
    return;
  }

  Window window = DefaultRootWindow(display);

  snprintf(status, sizeof(status), "%s | %s %s | %s | %s", data->keyboard,
           data->volume.main, data->volume.micro, data->brightness,
           data->clock);

  XStoreName(display, window, status);
  XFlush(display);

  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile uint8_t is_terminated = 0;

void *clockThread(void *param) {
  struct Data *data = (struct Data *)param;

  while (!is_terminated) {
    pthread_mutex_lock(&mutex);
    Clock(data->clock, sizeof(data->clock), "%d.%m.%Y %H.%M.%S");
    update_data(data);
    pthread_mutex_unlock(&mutex);
    sleep_ms(1000);
  }

  pthread_exit(NULL);
}

void *keyboardThread(void *param) {
  struct Data *data = (struct Data *)param;

  while (!is_terminated) {
    pthread_mutex_lock(&mutex);
    Keyboard(data->keyboard, sizeof(data->keyboard));
    update_data(data);
    pthread_mutex_unlock(&mutex);

    sleep_ms(1000);
  }

  pthread_exit(NULL);
}

void *brightnessThread(void *param) {
  struct Data *data = (struct Data *)param;

  while (!is_terminated) {
    pthread_mutex_lock(&mutex);
		Brightness(data->brightness, sizeof(data->brightness), "intel_backlight");
    update_data(data);
    pthread_mutex_unlock(&mutex);

    sleep_ms(1000);
  }

  pthread_exit(NULL);
}

void *volumeThread(void *param) {
  struct Data *data = (struct Data *)param;

  while (!is_terminated) {
    pthread_mutex_lock(&mutex);
    Audio(data->volume.main, sizeof(data->volume.main), "default", 0, "Master");
    Audio(data->volume.micro, sizeof(data->volume.micro), "default", 0, "Capture");
    update_data(data);
    pthread_mutex_unlock(&mutex);

    sleep_ms(1000);
  }

  pthread_exit(NULL);
}

void signal_handler(int sig) {
  switch (sig) {
  case SIGTERM:
  case SIGINT:
    printf("Terminating...\n");
    is_terminated = 1;
    break;
  default:
    break;
  }
}

int main(void) {
  struct sigaction sa = {0};
  sa.sa_handler = signal_handler;
  sigaction(SIGINT, &sa, NULL);

  struct Data data = {0};

  pthread_t clockId, keyboardId, volumeId, brightnessId;

  pthread_create(&clockId, NULL, clockThread, &data);
  pthread_create(&keyboardId, NULL, keyboardThread, &data);
  pthread_create(&volumeId, NULL, volumeThread, &data);
  pthread_create(&brightnessId, NULL, brightnessThread, &data);

  pthread_join(clockId, NULL);
  pthread_join(keyboardId, NULL);
  pthread_join(volumeId, NULL);
  pthread_join(brightnessId, NULL);

  return 0;
}
