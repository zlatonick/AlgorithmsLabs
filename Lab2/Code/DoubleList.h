#pragma once
#include "globals.h"

// Element of list structure
struct ListElement {
	int data;

	ListElement *next;
	ListElement *prev;

	ListElement(int _data, ListElement *_next, ListElement *_prev) :
		data(_data), next(_next), prev(_prev) {}
};

// List iterator - to go throw the list
class ListIterator {
	ListElement *currELement;

public:
	ListIterator(ListElement *head) {
		this->currELement = new ListElement(0, head, nullptr);
	}

	// True if trere is an element after current
	bool isNext() {
		return this->currELement->next != nullptr;
	}

	// Go to next element and return it
	int getNext() {
		this->currELement = this->currELement->next;
		return this->currELement->data;
	}
};

class DoubleList {
	ListElement *head;
	int size;

public:
	DoubleList() : head(nullptr), size(0) {}

	// Generate list of random elements
	DoubleList(int size);

	~DoubleList();

	int getSize();

	// Adding element to the beginning of the list
	void addElement(int data);
	
	// Get iterator of the list
	ListIterator* getIterator();

	void print();
};