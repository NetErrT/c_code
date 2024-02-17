#pragma once
#include <stdint.h>
#include <stdlib.h>

#define ARRAY_LENGTH(array) sizeof(array) / sizeof(array[0])

void sleep_ms(uint16_t miliseconds);
int is_string_in_array(const char *array[], size_t size, const char *string);

