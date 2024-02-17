#include <limits.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  char *label;
  long size;
  long long max;
} Type;

#define TYPE_BASE(type) #type, sizeof(type)

int main(void) {
  Type types[] = {
      {TYPE_BASE(int), INT_MAX},
      {TYPE_BASE(uint8_t), UINT8_MAX},
      {TYPE_BASE(uint16_t), UINT16_MAX},
      {TYPE_BASE(uint32_t), UINT32_MAX},
      {TYPE_BASE(uint64_t), UINT64_MAX},
      {TYPE_BASE(char), CHAR_MAX},
      {TYPE_BASE(unsigned char), UCHAR_MAX},
      {TYPE_BASE(long), LONG_MAX},
      {TYPE_BASE(unsigned long), ULONG_MAX},
      {TYPE_BASE(long long), LLONG_MAX},
      {TYPE_BASE(unsigned long long), ULLONG_MAX},
      {TYPE_BASE(void *), 0},
  };

  for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
    Type type = types[i];

    printf("%s { bytes = %lu, max = %llu }\n", type.label, type.size, type.max);
  }

  return 0;
}
