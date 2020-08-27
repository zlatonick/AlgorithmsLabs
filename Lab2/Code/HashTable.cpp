#include "HashTable.h"


HashTable::HashTable(int* arr, int size) {
	this->size = SIZE_COEF * size;
	this->table = new TElement[this->size];

	for (int i = 0; i < size; i++) {
		this->addToTable(TElement(arr[i], i));
	}
}


HashTable::HashTable(DoubleList *list) {
	this->size = SIZE_COEF * list->getSize();
	this->table = new TElement[this->size];

	ListIterator *it = list->getIterator();
	int index = 0;

	while (it->isNext()) {
		this->addToTable(TElement(it->getNext(), index));
		index++;
	}

	delete it;
}


HashTable::~HashTable() {
	delete[] this->table;
}


void HashTable::addToTable(TElement element) {
	int index = this->hashFunc(element.key);

	// Linear probing
	while (this->table[index].key != NOVALUE) {
		index++;

		if (index >= this->size) {
			index = 0;
		}
	}

	this->table[index] = element;
}


int HashTable::hashFunc(int key) {
	return key % this->size;
}


int HashTable::findElement(int key, int &comps, double &spentTime) {

	// Prepating for time counting
	LARGE_INTEGER startTime, endTime, frequency;
	QueryPerformanceFrequency(&frequency);

	// Starting the time counting
	QueryPerformanceCounter(&startTime);

	int index = this->hashFunc(key);

	while (this->table[index].key != key) {
		if (this->table[index].key == NOVALUE) {
			// Element not found
			return NOVALUE;
		}

		comps++;
		index++;

		if (index >= this->size) {
			index = 0;
		}
	}
	comps++;

	// Counting the time
	QueryPerformanceCounter(&endTime);

	// Counting time in milliseconds
	spentTime = ((long double)endTime.QuadPart - (long double)startTime.QuadPart) /
		(frequency.QuadPart / 1000.);	

	return this->table[index].oldIndex;
}


void HashTable::print() {
	std::cout << "Hash table:" << std::endl;

	for (int i = 0; i < this->size; i++) {
		std::cout << i << " - ";

		if (this->table[i].key != NOVALUE)
			std::cout << this->table[i].key;
		else
			std::cout << "Null";

		std::cout << std::endl;
	}

	std::cout << std::endl;
}