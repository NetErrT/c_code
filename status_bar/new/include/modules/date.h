#ifndef _MODULES_DATE_H
#define _MODULES_DATE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define DATE_SIZE_MAX 128

char *date_get(const char *format);

#endif
