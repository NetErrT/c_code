#include <stdio.h>

int main(void) {
  char a = 'a';
  char b = 'b';

  char *const pointer_to_a = &a;
  char const *const const_to_pointer = &a;

  pointer_to_a = &b;
  *pointer_to_a = b;

  const_to_pointer = &b;
  *const_to_pointer = b;

  printf("%c\n", *pointer_to_a);

  return 0;
}
