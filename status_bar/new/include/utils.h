#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stdlib.h>

int msleep(const unsigned long miliseconds);

uint8_t percent(const long current, const long max);

void *malloc_null(const size_t size);

#endif
