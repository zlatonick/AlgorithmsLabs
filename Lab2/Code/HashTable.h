#pragma once
#include "globals.h"
#include "DoubleList.h"

#define SIZE_COEF 4. / 3
#define NOVALUE -10000000

struct TElement {
	int key;
	int oldIndex;

	TElement() : key(NOVALUE), oldIndex(NOVALUE) {}
	TElement(int _key, int _oldIndex) :
		key(_key), oldIndex(_oldIndex) {}
};

class HashTable {
	TElement *table;
	int size;

	// Filling table with nulls
	void addToTable(TElement element);
	int hashFunc(int key);

public:
	HashTable(int* arr, int size);
	HashTable(DoubleList *list);

	~HashTable();

	void print();

	int findElement(int key, int &comps, double &spentTime);
};