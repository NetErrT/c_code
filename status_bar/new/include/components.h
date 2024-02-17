#ifndef _COMPONENTS_H
#define _COMPONENTS_H

typedef struct {
  void *(*component)(void *param);
  void *param;
} Arg;

typedef struct {
  char buffer[128];
  void *options;
} ThreadData;

extern void update(void);

#endif
