#include "Graph.h"


Graph::Graph(double** matr, int size) {
	this->verQuan = size;
	this->vertexes = new Vertex*[size];

	for (int i = 0; i < size; i++) {
		this->vertexes[i] = new Vertex;
		this->vertexes[i]->isVisited = false;
		this->vertexes[i]->id = i;
		this->vertexes[i]->adjQuan = 0;

		this->vertexes[i]->adjVertexes = new int[size];
		this->vertexes[i]->adjWeights = new double[size];

		for (int j = 0; j < size; j++) {
			if (matr[i][j] != -1) {
				this->vertexes[i]->adjVertexes[this->vertexes[i]->adjQuan] = j;
				this->vertexes[i]->adjWeights[this->vertexes[i]->adjQuan] = matr[i][j];

				this->vertexes[i]->adjQuan++;
			}
		}
	}
}


Graph::~Graph() {
	for (int i = 0; i < this->verQuan; i++) {
		delete this->vertexes[i]->adjVertexes;
		delete this->vertexes[i]->adjWeights;
	}
	delete this->vertexes;
}


void Graph::initHeuristic(double* h) {
	for (int i = 0; i < this->verQuan; i++) {
		this->vertexes[i]->h = h[i];
	}
}


double Graph::findDistanceHeuristic(int start, int finish, int* route, int& routeLen) {
	for (int i = 0; i < this->verQuan; i++) {
		this->vertexes[i]->isVisited = false;
	}

	double result = this->findDistanceHeuristicRec(start, finish, route, routeLen);

	for (int i = routeLen - 1; i >= 0; i--) {
		route[i + 1] = route[i];
	}
	route[0] = start;
	routeLen++;

	return result;
}


double Graph::findDistanceHeuristicRec(int start, int finish, int* route, int& routeLen) {
	
	if (start == finish) {
		return 0;
	}

	Vertex* currVertex = this->vertexes[start];
	currVertex->isVisited = true;

	int nextVertexID = 0;
	for (int i = 1; i < currVertex->adjQuan; i++) {
		if (this->vertexes[currVertex->adjVertexes[i]]->h
			< this->vertexes[currVertex->adjVertexes[nextVertexID]]->h
			&& !this->vertexes[currVertex->adjVertexes[i]]->isVisited) {

			nextVertexID = i;
		}
	}

	int nextVertex = currVertex->adjVertexes[nextVertexID];

	if (this->vertexes[nextVertex]->isVisited) {
		return 0;
	}

	route[routeLen] = nextVertex;
	routeLen++;

	return currVertex->adjWeights[nextVertexID] +
		this->findDistanceHeuristicRec(nextVertex, finish, route, routeLen);
}


class myLess {

public:
	bool operator () (const Vertex* v1, const Vertex* v2) const {
		return v1->f < v2->f;
	}
};


double Graph::findDistanceAStar(int start, int finish, int*& route, int& routeLen) {

	// Setting the isVisited
	for (int i = 0; i < this->verQuan; i++) {
		this->vertexes[i]->parent = -1;
		this->vertexes[i]->isVisited = false;
		this->vertexes[i]->f = 1000;
	}

	std::set<Vertex*, myLess> prQueue;

	// Distances from start vertex to all other vertexes
	double* g = new double[this->verQuan];
	for (int i = 0; i < this->verQuan; i++) {
		g[i] = 1000;
	}
	g[start] = 0;
	this->vertexes[start]->f = this->vertexes[start]->h;

	prQueue.insert(this->vertexes[start]);

	while (!prQueue.empty()) {
		Vertex* curr = *prQueue.begin();

		if (curr->id == finish) {
			// Getting the route
			int currID = curr->id;
			while (currID != -1) {
				route[routeLen] = currID;
				routeLen++;
				currID = this->vertexes[currID]->parent;
			}

			// Reversing the route
			int *newRoute = new int[routeLen];
			for (int i = 0; i < routeLen; i++) {
				newRoute[i] = route[routeLen - i - 1];
			}

			delete[] route;
			route = newRoute;

			return g[finish];
		}

		prQueue.erase(prQueue.begin());

		curr->isVisited = true;

		for (int i = 0; i < curr->adjQuan; i++) {
			int adj = curr->adjVertexes[i];
			double temp = g[curr->id] + curr->adjWeights[i];

			if (this->vertexes[adj]->isVisited && temp >= g[adj]) {
				continue;
			}
			if (/*!this->vertexes[adj]->isVisited || */temp < g[adj]) {
				this->vertexes[adj]->parent = curr->id;

				g[adj] = temp;
				this->vertexes[adj]->f = g[adj] + this->vertexes[adj]->h;

				std::set<Vertex*, myLess>::iterator it = prQueue.find(this->vertexes[adj]);
				if (it != prQueue.end()) {
					prQueue.erase(it);
					prQueue.insert(this->vertexes[adj]);

				}
				else {
					prQueue.insert(this->vertexes[adj]);
				}
			}
		}
	}

	return -1;
}