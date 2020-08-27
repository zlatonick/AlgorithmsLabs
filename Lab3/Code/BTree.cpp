#include "BTree.h";


BTree::BTree(int _branching, char* _fileName) {
	
	// Setting the branching
	this->branching = _branching;

	// Setting the offset
	// Root goes in the file after the branching,
	// address of the root and last offset
	this->currChildOffset = sizeof(int) + 2 * sizeof(long);

	// Setting the size of one block
	this->NODE_SIZE = sizeof(bool) + sizeof(int) +
		(2 * this->branching - 1) * sizeof(TData) +
		(2 * this->branching) * sizeof(long);

	// Creating the root node
	BTreeNode* newNode = new BTreeNode;

	newNode->id = this->nextOffset();
	newNode->isLeaf = true;
	newNode->keysQuan = 0;
	newNode->keys = this->createKeysArray();
	newNode->children = this->createChildrenArray();

	this->root = newNode;

	// Creating and filling the file
	this->file = fopen(_fileName, "wb+");
	
	// Writing the root
	this->writeNode(this->root);
}


BTree::BTree(char* _fileName) {

	// Reading tree from file
	this->file = fopen(_fileName, "rb+");

	// Reading the branching
	fread(&(this->branching), sizeof(int), 1, this->file);

	// Reading the address of the root
	long rootAddress;
	fread(&rootAddress, sizeof(long), 1, this->file);

	// Reading the offset
	fread(&(this->currChildOffset), sizeof(long), 1, this->file);

	// Setting the size of one block
	this->NODE_SIZE = sizeof(bool) + sizeof(int) +
		(2 * this->branching - 1) * sizeof(TData) +
		(2 * this->branching) * sizeof(long);

	// Reading the root
	this->root = BTree::readNode(rootAddress);
}


BTree::~BTree() {
	fseek(this->file, 0, SEEK_SET);

	// Writing the branching
	fwrite(&(this->branching), sizeof(int), 1, this->file);

	// Writing the root address
	fwrite(&(this->root->id), sizeof(long), 1, this->file);

	// Writing the current offset
	fwrite(&(this->currChildOffset), sizeof(long), 1, this->file);

	// Finishing the work
	this->emptifyNode(this->root);
	fclose(this->file);
}


int BTree::getBranching() {
	return this->branching;
}


long BTree::nextOffset() {
	long result = this->currChildOffset;
	this->currChildOffset += this->NODE_SIZE;
	return result;
}


TData** BTree::createKeysArray() {
	// Maximal size
	int size = 2 * this->branching - 1;
	
	TData** result = new TData*[size];

	for (int i = 0; i < size; i++) {
		result[i] = nullptr;
	}

	return result;
}


long* BTree::createChildrenArray() {
	// Maximal size
	int size = 2 * this->branching;

	long* result = new long[size];

	for (int i = 0; i < size; i++) {
		result[i] = 0;
	}

	return result;
}


void BTree::splitChild(BTreeNode* parent, BTreeNode* child, int index) {
	BTreeNode* newChild = new BTreeNode;

	newChild->id = this->nextOffset();
	newChild->isLeaf = child->isLeaf;

	newChild->keysQuan = this->branching - 1;
	newChild->keys = this->createKeysArray();
	newChild->children = this->createChildrenArray();

	// Putting the right half of keys and children to the new child
	for (int i = 0; i < newChild->keysQuan; i++) {
		newChild->keys[i] = new TData(*child->keys[i + this->branching]);
	}
	if (!child->isLeaf) {
		for (int i = 0; i < newChild->keysQuan + 1; i++) {
			newChild->children[i] = child->children[i + this->branching];
		}
	}

	// Changing the size of old child
	child->keysQuan = this->branching - 1;

	// Inserting the new child into the 'index' position
	for (int i = parent->keysQuan; i > index; i--) {
		parent->children[i + 1] = parent->children[i];
	}
	parent->children[index + 1] = newChild->id;

	// inserting the new kee into parent node
	for (int i = parent->keysQuan; i > index; i--) {
		parent->keys[i] = parent->keys[i - 1];
	}
	parent->keys[index] = new TData(*child->keys[this->branching - 1]);

	// Incrementing the size of parent
	parent->keysQuan++;

	// Writing all the nodes to the file
	this->writeNode(child);
	this->writeNode(newChild);
	this->writeNode(parent);

	// Emptifying the memory
	this->emptifyNode(newChild);
}


void BTree::insertNonFull(BTreeNode* currNode, TData* data) {
	int index = currNode->keysQuan - 1;

	if (currNode->isLeaf) {
		// Inserting data into array of keys
		while (index >= 0 && currNode->keys[index]->key > data->key) {
			currNode->keys[index + 1] = currNode->keys[index];
			index--;
		}
		currNode->keys[index + 1] = data;

		// Incrementing the quantity of keys
		currNode->keysQuan++;

		// Writing node to the file
		this->writeNode(currNode);
	}
	else {
		// Finding the needed child
		while (index >= 0 && currNode->keys[index]->key > data->key) {
			index--;
		}
		index++;

		// Reading the needed child
		BTreeNode* child = this->readNode(currNode->children[index]);

		// If needed child is full - splitting it
		if (child->keysQuan == 2 * this->branching - 1) {
			this->splitChild(currNode, child, index);

			// Choosing one of two childs
			if (data->key > currNode->keys[index]->key) {
				this->emptifyNode(child);
				child = this->readNode(currNode->children[index + 1]);
			}
		}

		// Inserting data into needed child
		this->insertNonFull(child, data);

		// Clearing the memory
		this->emptifyNode(child);
	}
}


int BTree::insert(TData* data) {

	// Checking if inserting key already exists
	BTreeNode* selectRes = nullptr;
	int compsQuan = 0;
	if (this->select(data->key, selectRes, compsQuan) != -1) {
		return 1;
	}

	// Emptifying the result of select
	if (selectRes != nullptr && selectRes != this->root) {
		this->emptifyNode(selectRes);
	}

	BTreeNode* r = this->root;

	// If the root is full
	if (r->keysQuan == 2 * this->branching - 1) {
		BTreeNode* newNode = new BTreeNode;

		newNode->id = this->nextOffset();
		newNode->isLeaf = false;
		newNode->keysQuan = 0;

		newNode->keys = this->createKeysArray();
		newNode->children = this->createChildrenArray();
		
		newNode->children[0] = r->id;

		this->root = newNode;

		this->splitChild(newNode, r, 0);
		this->emptifyNode(r);

		this->insertNonFull(newNode, data);
	}
	else {
		this->insertNonFull(r, data);
	}

	return 0;
}


void BTree::print() {
	std::cout << "B-Tree: " << std::endl;
	
	this->printRec(this->root);

	std::cout << std::endl;
}


void BTree::printRec(BTreeNode* node) {
	// Exit from recursion
	if (node == nullptr) {
		return;
	}

	// Printing the current node
	std::cout << "[";

	for (int i = 0; i < node->keysQuan; i++) {
		std::cout << "(" << node->keys[i]->key << ","
			<< node->keys[i]->value << ")";
	}

	std::cout << "]" << std::endl;

	// Printing the children of current node
	for (int i = 0; i < node->keysQuan + 1; i++) {
		if (node->children[i] != 0) {
			BTreeNode* child = this->readNode(node->children[i]);
			this->printRec(child);
		}
		else {
			break;
		}
	}
}


int BTree::select(int key, BTreeNode*& resultNode, int& compsQuan) {
	return this->selectRec(this->root, key, resultNode, compsQuan);
}



int BTree::selectRec(BTreeNode* node, int key, BTreeNode*& resultNode, int& compsQuan) {

	// Empty node
	if (node->keysQuan == 0) {
		return -1;
	}

	// Binary search in current node
	int lower = 0;
	int upper = node->keysQuan - 1;
	int index = 0;
	while (node->keys[index]->key != key && upper >= lower) {
		compsQuan++;

		index = (upper + lower) / 2;

		if (key < node->keys[index]->key) {
			upper = index - 1;
		}
		else {
			lower = index + 1;
		}
	}
	compsQuan++;

	// If found
	if (key == node->keys[index]->key) {
		resultNode = node;
		return index;
	}

	if (node->isLeaf) {
		// The tree doesn't contain received element
		return -1;
	}
	else {
		// Correcting the index of needed child
		if (node->keys[index]->key < key) {
			index++;
		}

		BTreeNode* child = this->readNode(node->children[index]);
		int result = this->selectRec(child, key, resultNode, compsQuan);

		if (child != resultNode) {
			this->emptifyNode(child);
		}

		return result;
	}
}


bool BTree::update(TData* newData) {
	BTreeNode* resultNode;
	int compsQuan = 0;
	int searchResult = this->select(newData->key, resultNode, compsQuan);

	if (searchResult == -1) {
		return false;
	}
	else {
		resultNode->keys[searchResult] = newData;
		this->writeNode(resultNode);

		if (resultNode != this->root) {
			this->emptifyNode(resultNode);
		}

		return true;
	}
}


void BTree::emptifyNode(BTreeNode* node) {
	for (int i = 0; i < node->keysQuan; i++) {
		delete node->keys[i];
	}

	delete[] node->keys;
	delete[] node->children;
	delete node;
}


bool BTree::deleteRecord(int key) {
	return this->deleteRec(this->root, key);
}


bool BTree::deleteRec(BTreeNode* node, int key) {

	// Checking if current node contains the received key.
	// Finding its index
	int index = -1;
	for (int i = 0; i < node->keysQuan; i++) {
		if (node->keys[i]->key == key) {
			index = i;
			break;
		}
	}

	// If current node contains the key
	if (index != -1) {

		// If current node is a leaf - simply removing the data
		if (node->isLeaf) {
			this->deleteKeyFromCurrNode(node, index);

			// Writing the node
			this->writeNode(node);
		}
		// If current node is an inner node
		else {
			
			// The child nodes lefts and rights the key
			BTreeNode* leftChild = this->readNode(node->children[index]);
			BTreeNode* rightChild = this->readNode(node->children[index + 1]);

			// Checking the left child node
			if (leftChild->keysQuan > this->branching - 1) {

				// Getting the key before received key
				// (last key of the left child)
				TData* newKey = new TData(*leftChild->keys[leftChild->keysQuan - 1]);

				// Replacing the key in current node with newKey
				node->keys[index] = newKey;

				// Writing the current node
				this->writeNode(node);

				// Deleting the newKey from the sub-tree
				this->deleteRec(leftChild, newKey->key);
			}
			// Checking the right child node
			else if (rightChild->keysQuan > this->branching - 1) {

				// Getting the key after received key
				// (first key of the right child)
				TData* newKey = new TData(*rightChild->keys[0]);

				// Replacing the key in current node with newKey
				node->keys[index] = newKey;

				// Writing the current node
				this->writeNode(node);

				// Deleting the newKey from the sub-tree
				this->deleteRec(rightChild, newKey->key);
			}
			// If both children have t-1 keys
			else {

				// Putting key and all keys from the right child
				// to the left child
				leftChild->keysQuan = 2 * this->branching - 1;
				leftChild->keys[this->branching - 1] = new TData(*node->keys[index]);
				
				for (int i = 0; i < rightChild->keysQuan; i++) {
					leftChild->keys[this->branching + i] = new TData(*rightChild->keys[i]);
				}

				// Also putting all the children of the right child
				// to the left child
				for (int i = 0; i < this->branching; i++) {
					leftChild->children[this->branching + i] =
						rightChild->children[i];
				}

				// Deleting the right child
				this->emptifyNode(rightChild);

				// Removing from current node key and pointer on right child
				this->deleteChildFromCurrNode(node, index + 1);
				this->deleteKeyFromCurrNode(node, index);

				// If current node is the root and contains now 0 keys -
				// reducing the height of the tree
				if (node == this->root && node->keysQuan == 0) {
					this->root = leftChild;
					this->emptifyNode(node);
				}
				else {
					this->writeNode(node);
				}

				// Recursive removing key from left child
				this->deleteRec(leftChild, key);
			}
		}

		// The key was found and removed - returning true
		return true;
	}
	// If node doesn't contain the key
	else {

		// If current node is a leaf - B-tree doesn't contain the key
		if (node->isLeaf) {
			return false;
		}

		// Finding the root of sub-tree that may contain the key
		// (Among the children of current node)
		int childIndex = 0;
		while (childIndex < node->keysQuan && key
				> node->keys[childIndex]->key) {
			childIndex++;
		}
		BTreeNode* newRoot = this->readNode(node->children[childIndex]);

		// If the child contains only t-1 keys
		if (newRoot->keysQuan == this->branching - 1) {

			BTreeNode* leftNeighbour = nullptr;
			BTreeNode* rightNeighbour = nullptr;

			if (childIndex > 0) {
				leftNeighbour = this->readNode(node->children[childIndex - 1]);
			}
			if (childIndex < node->keysQuan) {
				rightNeighbour = this->readNode(node->children[childIndex + 1]);
			}
			
			// Checking if neigbours of new root contain more, than t-1 keys
			// Checking the left neighbour
			if (leftNeighbour != nullptr &&
				leftNeighbour->keysQuan	> this->branching - 1) {

				// Putting the key between this two children into the new root
				for (int i = newRoot->keysQuan; i > 0; i--) {
					newRoot->keys[i] = newRoot->keys[i - 1];
				}
				newRoot->keys[0] = new TData(*node->keys[childIndex - 1]);
				newRoot->keysQuan++;

				// Putting the pointer from left neighbour to the new root
				for (int i = newRoot->keysQuan; i > 0; i--) {
					newRoot->children[i] = newRoot->children[i - 1];
				}
				newRoot->children[0] =
					leftNeighbour->children[leftNeighbour->keysQuan];

				// Replacing this key in the current node with the right key
				// from the left neighbour
				node->keys[childIndex - 1] = new TData(
					*leftNeighbour->keys[leftNeighbour->keysQuan - 1]);

				// Deleting the moved child and key from the left neighbour
				this->deleteChildFromCurrNode(leftNeighbour,
					leftNeighbour->keysQuan);
				this->deleteKeyFromCurrNode(leftNeighbour,
					leftNeighbour->keysQuan - 1);

				// Writing all modified nodes
				this->writeNode(node);
				this->writeNode(newRoot);
				this->writeNode(leftNeighbour);
			}
			// Checking the right neighbour
			else if (rightNeighbour != nullptr &&
				rightNeighbour->keysQuan > this->branching - 1) {

				// Putting the key between this two children into the new root
				newRoot->keys[newRoot->keysQuan] = new TData(*node->keys[childIndex]);
				newRoot->keysQuan++;

				// Putting the pointer from right neighbour to the new root
				newRoot->children[newRoot->keysQuan] =
					rightNeighbour->children[0];

				// Replacing this key in the current node with the left key
				// from the right neighbour
				node->keys[childIndex] = new TData(*rightNeighbour->keys[0]);

				// Deleting the moved child and key from the left neighbour
				this->deleteChildFromCurrNode(rightNeighbour, 0);
				this->deleteKeyFromCurrNode(rightNeighbour, 0);

				// Writing all modified nodes
				this->writeNode(node);
				this->writeNode(newRoot);
				this->writeNode(rightNeighbour);
			}
			// If both neighbours have t-1 keys
			else {

				// Merging two child nodes
				// Choosing the neighbour to merge
				if (rightNeighbour != nullptr) {

					// Merging with right neighbour
					// Adding the key between neighbours to the new root
					newRoot->keys[newRoot->keysQuan] = new TData(*node->keys[childIndex]);
					newRoot->keysQuan++;

					// Adding all the keys and pointers from the
					// right neighbour to the new root.
					// Changing the quantity of keys
					for (int i = 0; i < rightNeighbour->keysQuan; i++) {
						newRoot->keys[newRoot->keysQuan + i] =
							new TData(*rightNeighbour->keys[i]);
						newRoot->children[newRoot->keysQuan + i] =
							rightNeighbour->children[i];
					}
					newRoot->children[2 * this->branching - 1] =		// Last child
						rightNeighbour->children[this->branching - 1];

					newRoot->keysQuan = 2 * this->branching - 1;

					// Deleting the right neighbour
					this->emptifyNode(rightNeighbour);

					// Removing key and child from the current node
					this->deleteChildFromCurrNode(node, childIndex + 1);
					this->deleteKeyFromCurrNode(node, childIndex);

					// If current node is the root and contains now 0 keys -
					// reducing the height of the tree
					if (node == this->root && node->keysQuan == 0) {
						this->root = newRoot;
						this->emptifyNode(node);
					}
					else {
						this->writeNode(node);
					}

					// Writing all modified nodes
					this->writeNode(newRoot);
				}
				else {

					// Merging with left neighbour
					// Adding the key between neighbours to the new root
					for (int i = newRoot->keysQuan; i > 0; i--) {
						newRoot->keys[i] = newRoot->keys[i - 1];
					}
					newRoot->keys[0] = new TData(*node->keys[childIndex - 1]);
					newRoot->keysQuan++;

					// Adding all the keys and pointers from the
					// left neighbour to the new root.
					// Shifting the existing elements
					for (int i = newRoot->keysQuan - 1; i >= 0; i--) {
						newRoot->keys[i + leftNeighbour->keysQuan] =
							newRoot->keys[i];
						newRoot->children[i + leftNeighbour->keysQuan + 1] =
							newRoot->children[i];
					}

					// Inserting new elements
					for (int i = 0; i < leftNeighbour->keysQuan; i++) {
						newRoot->keys[i] = new TData(*leftNeighbour->keys[i]);
						newRoot->children[i] = leftNeighbour->children[i];
					}
					newRoot->children[this->branching - 1] =		// Last child
						leftNeighbour->children[this->branching - 1];

					// Changing the size
					newRoot->keysQuan = 2 * this->branching - 1;

					// Deleting the left neighbour
					this->emptifyNode(leftNeighbour);

					// Removing key and child from the current node
					this->deleteChildFromCurrNode(node, childIndex - 1);
					this->deleteKeyFromCurrNode(node, childIndex - 1);

					// If current node is the root and contains now 0 keys -
					// reducing the height of the tree
					if (node == this->root && node->keysQuan == 0) {
						this->root = newRoot;
						this->emptifyNode(node);
					}
					else {
						this->writeNode(node);
					}

					// Writing all modified nodes
					this->writeNode(newRoot);
				}
			}
		}

		// Recursive removing the key from the child
		return this->deleteRec(newRoot, key);
	}	
}


void BTree::deleteKeyFromCurrNode(BTreeNode* node, int index) {
	delete node->keys[index];

	for (int i = index; i < node->keysQuan - 1; i++) {
		node->keys[i] = node->keys[i + 1];
	}

	node->keys[node->keysQuan - 1] = nullptr;
	node->keysQuan--;
}


void BTree::deleteChildFromCurrNode(BTreeNode* node, int index) {
	for (int i = index; i < node->keysQuan; i++) {
		node->children[i] = node->children[i + 1];
	}
	node->children[node->keysQuan] = 0;
}


BTreeNode* BTree::readNode(long id) {

	// Creating the node
	BTreeNode* node = new BTreeNode;
	node->id = id;

	// Going to the child
	fseek(this->file, node->id, SEEK_SET);

	// Reading node parameters
	fread(&(node->isLeaf), sizeof(bool), 1, this->file);
	fread(&(node->keysQuan), sizeof(int), 1, this->file);

	// Reading the keys
	TData* keysBuffer = new TData[2 * this->branching - 1];
	fread(keysBuffer, sizeof(TData), 2 * this->branching - 1, this->file);

	node->keys = this->createKeysArray();
	for (int i = 0; i < node->keysQuan; i++) {
		node->keys[i] = new TData(keysBuffer[i]);
	}

	delete[] keysBuffer;

	// Reading the children
	node->children = this->createChildrenArray();
	fread(node->children, sizeof(long), 2 * this->branching, this->file);

	return node;
}


void BTree::writeNode(BTreeNode* node) {

	// Going to the needed position
	fseek(this->file, node->id, SEEK_SET);

	// Writing node parameters
	fwrite(&(node->isLeaf), sizeof(bool), 1, this->file);
	fwrite(&(node->keysQuan), sizeof(int), 1, this->file);

	// Writing the keys
	TData* keysBuffer = new TData[2 * this->branching - 1];

	for (int i = 0; i < node->keysQuan; i++) {
		keysBuffer[i] = *(node->keys[i]);
	}

	// Writing the maximal number of keys
	fwrite(keysBuffer, sizeof(TData), 2 * this->branching - 1, this->file);

	delete[] keysBuffer;

	// Writing the children
	fwrite(node->children, sizeof(long), 2 * this->branching, this->file);

	fflush(this->file);
}


std::string BTree::toString() {

	std::string result = "digraph g {\
        graph [\
                rankdir = \"TD\"\
        ];\
        node [\
                fontsize = \"16\"\
                shape = \"ellipse\"\
        ];\
        edge [\
        ];\n";

	this->toStringRec(result, this->root);

	result += "}";

	return result;
}


void BTree::toStringRec(std::string& line, BTreeNode* currNode) {

	line += "\"node" + std::to_string(currNode->id) + "\" [\nlabel=\"";

	// Printing keys
	for (int i = 0; i < currNode->keysQuan; i++) {
		line += "<f" + std::to_string(i) + "> | " + 
			std::to_string(currNode->keys[i]->key) + " | ";
	}
	line += "<f" + std::to_string(currNode->keysQuan) +
		">\"\nshape = \"record\"\n];\n";

	if (!currNode->isLeaf) {

		// Printing links
		for (int i = 0; i < currNode->keysQuan + 1; i++) {
			line += "\"node" + std::to_string(currNode->id) + "\":f" + 
				std::to_string(i) + " -> \"node" +
				std::to_string(currNode->children[i]) + "\";\n";
		}

		// Printing children
		for (int i = 0; i < currNode->keysQuan + 1; i++) {
			BTreeNode* child = this->readNode(currNode->children[i]);
			this->toStringRec(line, child);
			this->emptifyNode(child);
		}
	}	
}