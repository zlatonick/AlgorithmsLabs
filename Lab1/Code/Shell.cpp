#include "Shell.h"

std::vector<int> generateLengths(int size) {
	int index = 1;

	std::vector<int> lens;

	// First value
	int currLen = 1;

	while (3 * currLen <= size) {
		lens.push_back(currLen);

		if (index % 2 == 0) {
			currLen = 9 * pow(2, index) - 9 * pow(2, index / 2) + 1;
		}
		else {
			currLen = 8 * pow(2, index) - 6 * pow(2, (index + 1) / 2) + 1;
		}

		index++;
	}

	return lens;
}


double shellSort(int *arr, int size, std::vector<int> lens, int& comps, int& shifts) {
	int lensSize = lens.size();

	// Prepating for time counting
	LARGE_INTEGER startTime, endTime, frequency;
	QueryPerformanceFrequency(&frequency);

	// Starting the time counting
	QueryPerformanceCounter(&startTime);

	// For each length, beginning with the biggest
	for (int k = lensSize - 1; k >= 0; k--) {
		int len = lens[k];

		for (int i = 0; i < len; i++) {
			for (int j = i + len; j < size; j += len) {
				int curr = arr[j];

				int temp = j - len;

				while (temp >= 0 && arr[temp] > curr) {
					arr[temp + len] = arr[temp];
					temp -= len;

					comps++;
					shifts++;
				}

				// Last compare
				if (temp >= 0) {
					comps++;
				}

				arr[temp + len] = curr;
			}
		}
	}

	// Counting the time
	QueryPerformanceCounter(&endTime);

	// Returning time in milliseconds
	return ((long double)endTime.QuadPart - (long double)startTime.QuadPart) /
		(frequency.QuadPart / 1000.);
}


double insertionSort(int *arr, int size, int& comps, int& shifts) {
	// Prepating for time counting
	LARGE_INTEGER startTime, endTime, frequency;
	QueryPerformanceFrequency(&frequency);

	// Starting the time counting
	QueryPerformanceCounter(&startTime);

	// For each length, beginning with the biggest
	for (int i = 1; i < size; i++) {
		int curr = arr[i];

		int j = i - 1;
		while (j >= 0 && arr[j] > curr) {
			arr[j + 1] = arr[j];
			j--;

			comps++;
			shifts++;
		}

		// Last compare
		if (j >= 0) {
			comps++;
		}

		arr[j + 1] = curr;
	}

	// Counting the time
	QueryPerformanceCounter(&endTime);

	// Returning time in milliseconds
	return ((long double)endTime.QuadPart - (long double)startTime.QuadPart) /
		(frequency.QuadPart / 1000.);
}