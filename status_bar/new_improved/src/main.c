#include <stdio.h>
#include <stdlib.h>

#define lengthof(array) (sizeof(array) / sizeof((array)[0]))

int main(void) {
	const int fr[] = {1, 2, 4};

	printf("%lu\n", lengthof(fr));

	return EXIT_SUCCESS;
}
