#pragma once
#include "globals.h";
#include "TData.h"

struct BTreeNode {
	long id;			// Address of node in the file

	bool isLeaf;
	int keysQuan;

	TData** keys;

	long* children;
};

class BTree {

	// The root of tree
	BTreeNode *root;

	// Minimal quantity of childen of non-root nodes
	int branching;

	// Unique ID of child (its address in the file)
	long currChildOffset;

	// File of B-Tree
	FILE* file;

	// The size of one block of data in file
	long NODE_SIZE;

	// Returns current offset and increments it
	long nextOffset();

	TData** createKeysArray();
	long* createChildrenArray();

	/**
	 * Splitting the node
	 * @param parent The parent of node that will be splitted. Is not full.
	 * @param child The node that will be splitted. Is full
	 * @param index The index of child node in the array of childs of parent node
	 */
	void splitChild(BTreeNode* parent, BTreeNode* child, int index);

	/**
	 * Inserting data into the sub-tree with non-full root
	 */
	void insertNonFull(BTreeNode* currNode, TData* data);

	int selectRec(BTreeNode* node, int key, BTreeNode*& resultNode, int& compsQuan);

	// Deleting the memory from node
	void emptifyNode(BTreeNode* node);

	bool deleteRec(BTreeNode* node, int key);

	void deleteKeyFromCurrNode(BTreeNode* node, int index);
	void deleteChildFromCurrNode(BTreeNode* node, int index);

	void printRec(BTreeNode* node);
	
	// Reading the node from file
	BTreeNode* readNode(long id);

	// Writing the node into the file
	void writeNode(BTreeNode* node);

	void toStringRec(std::string& line, BTreeNode* currNode);

public:

	// Constructor that creates the new B-Tree
	BTree(int _branching, char* _fileName);

	// Constructor that reads B-Tree from file
	BTree(char* _fileName);

	~BTree();

	int getBranching();

	int select(int key, BTreeNode*& resultNode, int& compsQuan);

	int insert(TData* data);

	bool update(TData* newData);

	bool deleteRecord(int key);

	std::string toString();

	// DEBUG
	void print();
};