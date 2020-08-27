#include "globals.h"
#include "ArrayHandler.h"

int* generateArray(int size) {
	int *result = new int[size];

	for (int i = 0; i < size; i++) {
		result[i] = rand() % (size * 10);
	}

	return result;
}


void outputArray(int *arr, int size) {
	std::cout << "Array:" << std::endl;

	for (int i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}

	std::cout << std::endl;
}