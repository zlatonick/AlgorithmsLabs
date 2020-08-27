#include "globals.h"
#include "Generator.h"
#include "Shell.h"

void outputArray(int *arr, int size);

int main() {
	int size = 10000;

	int *arr = straightArray(size);
	//int *arr = reverseArray(size);
	//int *arr = randomArray(size);
	
	//outputArray(arr, size);

	std::vector<int> lengthArray = generateLengths(size);
	
	int comps = 0, shifts = 0;
	double spentTime = shellSort(arr, size, lengthArray, comps, shifts);
	//double spentTime = insertionSort(arr, size, comps, shifts);

	//outputArray(arr, size);

	std::cout << "Size of array: " << size << std::endl;
	std::cout << "Spent time (milliseconds): " << spentTime << std::endl;
	std::cout << "Quantity of compares: " << comps << std::endl;
	std::cout << "Quantity of shifts: " << shifts << std::endl;

	delete[] arr;

	system("pause");
}


void outputArray(int *arr, int size) {
	std::cout << "Array:" << std::endl;

	for (int i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}

	std::cout << std::endl;
}