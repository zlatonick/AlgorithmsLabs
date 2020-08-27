#pragma once
#include "globals.h"

// Class that stores information about the graph
class Graph {
	
	// The quantity of vertexes
	int versQuan;

	// Adjacency matrix
	int** matr;

	// The quantity of bees
	const int beesQuantity = 28;		// 30 - 2 scouts

	// The quantity of iterations
	const int iterationQuan = 1000;

	// Finding the bee on the site with the least value of the function
	// (the least quantity of colours)
	int* findTheFittestBee(int* center, int beesQuan);

	// Checking if the painting of graph is correct
	bool checkPainting(int* colours);

	// Normalizing the correct painting (minimizing the maximum colour)
	void normalizePainting(int* colours, int doubtColour);

public:

	// Constructor that reads graph from the file
	Graph(char* fileName);

	// Destructor
	~Graph();

	// Greedy painting of graph. 
	// Returns the colour id of each vertex
	int* paintGreedy(std::vector<int> order);

	// Bee-painting of graph. 
	// Returns the colour id of each vertex
	int* paintBees();

	// Find the quantity of colours of the normalized painting
	int coloursQuan(int* colours);

	// Get the quantity of vertexes of the graph
	int getVersQuan();

	// Output the adjacency matrix of the graph
	void print();

	// Generating the graph and putting it into the file
	static void generateGraph(int versQuan, int maxPower, char* fileName);
};