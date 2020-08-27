#include "globals.h"
#include "HashTable.h"
#include "ArrayHandler.h"
#include "DoubleList.h"

int main() {
	srand(time(NULL));

	int size = 0;
	int arrayOrList = 0;

	std::cout << "Enter the size: ";
	std::cin >> size;
	std::cout << "Array(1) or list(2): ";
	std::cin >> arrayOrList;

	HashTable *hashTable;

	int *arr;
	DoubleList *list;

	if (arrayOrList == 1) {
		arr = generateArray(size);
		outputArray(arr, size);

		hashTable = new HashTable(arr, size);
	}
	else {
		list = new DoubleList(size);
		list->print();

		hashTable = new HashTable(list);
	}
		
	//hashTable.print();

	int element;
	std::cout << "Enter the element to find (-1 to stop): ";
	std::cin >> element;

	while (element != -1) {
		int compsQuan = 0;
		double spentTime = 0;

		double index = hashTable->findElement(element, compsQuan, spentTime);

		if (index != NOVALUE) {
			std::cout << "Index of element: " << index << std::endl;
			std::cout << "Quantity of compares: " << compsQuan << std::endl;
			std::cout << "Spent time: " << spentTime << std::endl;
		}
		else {
			std::cout << "Element not found" << std::endl;
		}

		std::cout << "Enter the element to find (-1 to stop): ";
		std::cin >> element;
	}		

	if (arrayOrList == 1) {
		delete[] arr;
	}
	else {
		delete list;
	}
	delete hashTable;

	system("pause");
}