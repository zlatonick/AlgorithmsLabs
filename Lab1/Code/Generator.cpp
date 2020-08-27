#include "Generator.h"

int* straightArray(int size) {
	int* result = new int[size];

	for (int i = 0; i < size; i++) {
		result[i] = i;
	}

	return result;
}


int* reverseArray(int size) {
	int* result = new int[size];

	for (int i = 0; i < size; i++) {
		result[i] = size - i - 1;
	}

	return result;
}


int* randomArray(int size) {
	srand(time(NULL));

	int* result = new int[size];

	for (int i = 0; i < size; i++) {
		result[i] = rand() % (size * 3);
	}

	return result;
}