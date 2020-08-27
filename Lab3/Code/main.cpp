#include "globals.h"
#include "TData.h"
#include "BTree.h"

#define BRANCHING 3

char* fileName = "data.bin";

int main() {
	
	BTree* tree = new BTree(BRANCHING, fileName);
	//BTree* tree = new BTree(fileName);

	tree->insert(new TData(0, 16));
	tree->insert(new TData(1, 18));
	tree->insert(new TData(2, 20));
	tree->insert(new TData(3, 22));
	tree->insert(new TData(4, 24));
	/*tree->insert(new TData(5, 26));
	tree->insert(new TData(6, 28));
	tree->insert(new TData(7, 30));
	tree->insert(new TData(8, 32));
	tree->insert(new TData(9, 2));
	tree->insert(new TData(10, 4));
	tree->insert(new TData(11, 6));
	tree->insert(new TData(12, 8));
	tree->insert(new TData(13, 10));
	tree->insert(new TData(14, 12));
	tree->insert(new TData(15, 14));*/

	/*for (int i = 0; i < 2800; i++) {
		std::cout << (i + 1) << std::endl;
		tree->insert(new TData(i, 2 * i));
	}

	tree->insert(new TData(2800, 2 * 2800));
	tree->insert(new TData(2801, 2 * 2801));
	tree->insert(new TData(2802, 2 * 2802));
	tree->insert(new TData(2803, 2 * 2803));*/

	//tree->print();
	
	//std::cout << tree->toString() << std::endl;

	//tree->deleteRecord(99);
	//tree->print();

	/*tree->deleteRecord(10);
	tree->print();*/

	/*tree->update(new TData(9, 209));

	tree->print();*/

	/*BTreeNode* selectRes;
	int compsQuan = 0;
	int index = tree->select(3, selectRes, compsQuan);

	std::cout << selectRes->keys[index]->value << std::endl;
	std::cout << "The quantity of compares: " << compsQuan << std::endl;*/
	//tree->update(new TData(4, 'q'));

	//tree->deleteRecord(13);

	//tree->test();

	delete tree;

	system("pause");
}