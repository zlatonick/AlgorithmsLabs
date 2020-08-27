#include "DoubleList.h"


DoubleList::DoubleList(int size) {
	this->head = nullptr;
	this->size = 0;
	
	for (int i = 0; i < size; i++) {
		this->addElement(rand() % (size * 10));
	}
}


DoubleList::~DoubleList() {
	while (this->head != nullptr) {
		ListElement *curr = this->head;
		this->head = this->head->next;
		delete curr;
	}
}


int DoubleList::getSize() {
	return this->size;
}


void DoubleList::addElement(int data) {
	ListElement *newElement = new ListElement(data, this->head, nullptr);

	if (this->head != nullptr) {
		this->head->prev = newElement;
	}
	this->head = newElement;
	this->size++;
}


ListIterator* DoubleList::getIterator() {
	return new ListIterator(this->head);
}


void DoubleList::print() {
	ListElement *curr = this->head;

	std::cout << "List:" << std::endl;

	while (curr != nullptr) {
		std::cout << curr->data << " ";
		curr = curr->next;
	}

	std::cout << std::endl;
}