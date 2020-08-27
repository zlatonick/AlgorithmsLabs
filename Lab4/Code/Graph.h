#pragma once
#include "globals.h"

struct Vertex {
	bool isVisited;
	int id;

	int parent;

	double h;
	double f;

	int adjQuan;

	int* adjVertexes;
	double* adjWeights;
};

class Graph {
	Vertex** vertexes;
	int verQuan;

	double findDistanceHeuristicRec(int start, int finish, int* route, int& routeLen);

public:
	Graph(double** matr, int size);
	~Graph();

	void initHeuristic(double* h);

	double findDistanceHeuristic(int start, int finish, int* route, int& routeLen);

	double findDistanceAStar(int start, int finish, int*& route, int& routeLen);
};