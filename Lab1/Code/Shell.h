#pragma once
#include "globals.h"

// Generating the Sedgewick sequence of lengths
std::vector<int> generateLengths(int size);

// Sorting the array (returning the time in milliseconds)
double shellSort(int *arr, int size, std::vector<int> lens, int& comps, int& shifts);

// Insertion sort
double insertionSort(int *arr, int size, int& comps, int& shifts);