#include "header.h"

void hello () {
	printf("Hello world\n");
}

float mean (int size, int *num) {
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += num[i];
	}
	sum /= size;
	return sum;
}

int median (int size, int *num) {
	return 0;

}
