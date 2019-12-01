#include "header.h"

void hello () {
	printf("Hello world\n");
}

int mean (RECORD *data) {
    if (data[MAX_RECORDS].ID <= 0) {
        return -1;
    }    
    int sum = 0;
    for (int i = 0; i < data[MAX_RECORDS].ID; i++) {
            sum += data[i].salary;
    }
    sum /= data[MAX_RECORDS].ID;
    return sum;
}

int max_value (RECORD *data, int *index) {
    if (data[MAX_RECORDS].ID <= 0) {
        return -1;
    }    
    int max = data[0].salary;
    *index = 0;
    for (int i = 0; i < data[MAX_RECORDS].ID; i++) {
        if(max < data[i].salary) {
            max = data[i].salary;
            *index = i;
        }
    }
    return max;
}

int min_value (RECORD *data, int *index) {
    if (data[MAX_RECORDS].ID <= 0) {
        return -1;
    }    
    int min = data[0].salary;
    *index = 0;
    for (int i = 0; i < data[MAX_RECORDS].ID; i++) {
        if(min > data[i].salary) {
            min = data[i].salary;
            *index = i;
        }
    }
    return min;
}

int median (int size, int *num) {
	return 0;
}
