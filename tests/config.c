#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t size;
  size_t threads_size;
} ThreadData;

static size_t size = 0;
static ThreadData **threads_data = NULL;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void print_size(void) {
  pthread_mutex_lock(&mutex);
  for (size_t i = 0; i < size; i++) {
    const ThreadData *const thread = threads_data[i];

    if (!thread)
      continue;

    printf("%zu\n", thread->size);
  }
  pthread_mutex_unlock(&mutex);
}

void *second_thread(void *const param) {
  if (param) {
    print_size();
  }

  pthread_exit(NULL);
}

void cleanup(void) {
  for (size_t i = 0; i < size; i++) {
    ThreadData *thread_data = threads_data[i];

    if (!thread_data)
      continue;

    free(thread_data);
  }

  free(threads_data);
}

int main(void) {
  size = 3;

  pthread_t threads[size];

  threads_data = malloc(size * sizeof(ThreadData *));

  for (size_t i = 0; i < size; i++) {
    ThreadData *thread_data = malloc(sizeof(ThreadData));
    thread_data->size = i;
    thread_data->threads_size = size;

    pthread_t id;

    if (pthread_create(&id, NULL, second_thread, thread_data)) {
      perror("Failed to create thread");
      return EXIT_FAILURE;
    }

    threads[i] = id;
    threads_data[i] = thread_data;
  }

  for (size_t i = 0; i < size; i++) {
    pthread_t thread = threads[i];

    pthread_join(thread, NULL);
  }

  cleanup();

  return EXIT_SUCCESS;
}
