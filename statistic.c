#include "header.h"

void hello () {
	printf("Hello world\n");
}

int mean (RECORD *data) {
	int sum = 0;
	for (int i = 0; i < data[MAX_RECORDS].ID; i++) {
		sum += data[i].salary;
	}
	sum /= data[MAX_RECORDS].ID;
	return sum;
}

int median (int size, int *num) {
	return 0;

}
