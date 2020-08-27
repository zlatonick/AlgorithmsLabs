#include "Graph.h"


// Constructor that reads graph from the file
Graph::Graph(char* fileName) {

	// Opening the file
	FILE* file = fopen(fileName, "rt");

	// Reading the quantity of vertexes and edges
	int edgeQuan;

	fscanf(file, "%d %d\n", &this->versQuan, &edgeQuan);

	// Creating and filling the adjacency matrix
	this->matr = new int*[this->versQuan];

	for (int i = 0; i < this->versQuan; i++) {
		this->matr[i] = new int[this->versQuan];

		for (int j = 0; j < this->versQuan; j++) {
			this->matr[i][j] = 0;
		}
	}

	// Reading the edges
	for (int i = 0; i < edgeQuan; i++) {
		int start;
		int end;

		// Reading the edge
		fscanf(file, "%d %d\n", &start, &end);

		// Normalizing the edge
		start--;
		end--;

		// Putting into adjacency matrix
		this->matr[start][end] = 1;
		this->matr[end][start] = 1;
	}

	// Closing the file
	fclose(file);
}


// Destructor
Graph::~Graph() {
	for (int i = 0; i < this->versQuan; i++) {
		delete[] this->matr[i];
	}
	delete[] this->matr;
}


// Greedy painting of graph. 
// Returns the colour id of each vertex
int* Graph::paintGreedy(std::vector<int> order) {
	
	// Creating the result array of colours
	int* colour = new int[this->versQuan];

	for (int i = 0; i < this->versQuan; i++) {
		colour[i] = -1;
	}

	// Current maximal colour index in the graph
	int currMaxColour = 0;

	// Greedy algorythm
	int size = order.size();
	for (int i = 0; i < size; i++) {
		int index = order[i];

		for (int j = 0; j <= currMaxColour; j++) {
			// Checking if can paint the vertex into this colour
			bool canPaint = true;

			for (int k = 0; k < this->versQuan; k++) {
				if (this->matr[index][k] == 1 && colour[k] == j) {
					canPaint = false;
					break;
				}
			}

			if (canPaint) {
				colour[index] = j;
				break;
			}
		}

		// If still not painted
		if (colour[index] == -1) {
			// Increasing the maximal colour and using it
			currMaxColour++;
			colour[index] = currMaxColour;
		}
	}

	return colour;
}


// Bee-painting of graph. 
// Returns the colour id of each vertex
int* Graph::paintBees() {

	// Generating the random order of vertexes
	std::vector<int> order(this->versQuan);
	for (int i = 0; i < this->versQuan; i++) {
		order[i] = i;
	}

	std::random_device rd;
	std::mt19937 g(rd());
	
	// First scout bee
	std::shuffle(order.begin(), order.end(), g);
	int* scoutBee1 = this->paintGreedy(order);

	// Second scout bee
	std::shuffle(order.begin(), order.end(), g);
	int* scoutBee2 = this->paintGreedy(order);

	int currIteration = 0;
	while (currIteration < this->iterationQuan) {

		// Finding the lowest (scoutBee1 becomes the lowest)
		if (this->coloursQuan(scoutBee1) > this->coloursQuan(scoutBee2)) {
			delete[] scoutBee1;
			scoutBee1 = scoutBee2;
		}
		else {
			delete[] scoutBee2;
		}

		// Finding the fittest bee on the site
		int* fittestBee = this->findTheFittestBee(scoutBee1, this->beesQuantity);

		// Setting the new site center
		delete[] scoutBee1;
		scoutBee1 = fittestBee;

		// Assigning the remained bee
		std::shuffle(order.begin(), order.end(), g);
		scoutBee2 = this->paintGreedy(order);

		currIteration++;

		// To track the iterations
		if (currIteration % 20 == 0) {
			std::cout << currIteration << " iterations" << std::endl;
			std::cout << "Current quantity of colours: "
				<< this->coloursQuan(scoutBee1) << std::endl;
		}
	}

	return scoutBee1;
}


// Finding the bee on the site with the least value of the function
// (the least quantity of colours)
int* Graph::findTheFittestBee(int* center, int beesQuan) {
	
	// Result vector of colours
	int* result = new int[this->versQuan];
	for (int i = 0; i < this->versQuan; i++) {
		result[i] = center[i];
	}

	// The quantity of colours of the result vector
	int resultQuan = this->coloursQuan(center);

	// Current index in the array of colours
	int currIndex = 0;

	// Current quantity of probed bees
	int currBeeQuan = 0;

	// Buffer to store the current painting
	int* buffer = new int[this->versQuan];

	// Going through all the colours in received colour vector
	while (currIndex < this->versQuan && currBeeQuan < beesQuan) {

		// Filling the buffer
		for (int i = 0; i < this->versQuan; i++) {
			buffer[i] = center[i];
		}
		
		// Incrementing the parameter
		buffer[currIndex]++;

		// If received painting is correct
		if (this->checkPainting(buffer)) {

			// New bee
			currBeeQuan++;

			// Normalizing the painting
			this->normalizePainting(buffer, center[currIndex]);

			// The quantity of colours of the current bee
			int bufferResult = this->coloursQuan(buffer);

			// If current bee is now the fittest
			if (bufferResult < resultQuan) {
				
				// Copying buffer to result
				for (int i = 0; i < this->versQuan; i++) {
					result[i] = buffer[i];
				}

				// Saving its quantity of colours
				resultQuan = bufferResult;
			}
		}

		// Returning the initial vector
		for (int i = 0; i < this->versQuan; i++) {
			buffer[i] = center[i];
		}

		// Decrementing the parameter
		buffer[currIndex]--;

		// Positive colours only
		if (buffer[currIndex] < 0) {
			currIndex++;
			continue;
		}

		// If received painting is correct
		if (this->checkPainting(buffer)) {

			// New bee
			currBeeQuan++;

			// Normalizing the painting
			this->normalizePainting(buffer, center[currIndex]);

			// The quantity of colours of the current bee
			int bufferResult = this->coloursQuan(buffer);

			// If current bee is now the fittest
			if (bufferResult < resultQuan) {

				// Copying buffer to result
				for (int i = 0; i < this->versQuan; i++) {
					result[i] = buffer[i];
				}

				// Saving its quantity of colours
				resultQuan = bufferResult;
			}
		}

		// Going to the next element of vector
		currIndex++;
	}

	return result;
}


// Checking if the painting of graph is correct
bool Graph::checkPainting(int* colours) {

	// Going through all the vertexes
	for (int i = 0; i < this->versQuan; i++) {

		// The colour of the current vertex
		int currColour = colours[i];

		// Checking if there is any adjacent vertex with the same colour
		for (int j = i + 1; j < this->versQuan; j++) {
			if (this->matr[i][j] == 1 && colours[i] == colours[j]) {
				return false;
			}
		}
	}
	return true;
}


// Normalizing the correct painting (minimizing the maximum colour)
void Graph::normalizePainting(int* colours, int doubtColour) {

	// Checking if painting needs normalizing
	for (int i = 0; i < this->versQuan; i++) {
		if (colours[i] == doubtColour) {
			// Doesn't need normalizing
			return;
		}
	}

	// Moving all the colours that are bigger
	for (int i = 0; i < this->versQuan; i++) {
		if (colours[i] > doubtColour) {
			colours[i]--;
		}
	}
}


// Find the quantity of colours of the normalized painting
int Graph::coloursQuan(int* colours) {

	int result = colours[0];

	// Finding the biggest colour
	for (int i = 0; i < this->versQuan; i++) {
		if (colours[i] > result) {
			result = colours[i];
		}
	}

	// Quantity of colours is the incremented biggest colour
	return result + 1;
}


// Get the quantity of vertexes of the graph
int Graph::getVersQuan() {
	return this->versQuan;
}


// Output the adjacency matrix of the graph
void Graph::print() {
	std::cout << "Adjacency matrix:" << std::endl;

	for (int i = 0; i < this->versQuan; i++) {
		for (int j = 0; j < this->versQuan; j++) {
			std::cout << this->matr[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


// Generating the graph and putting it into the file
void Graph::generateGraph(int versQuan, int maxPower, char* fileName) {

	std::random_device rd;
	std::mt19937 g(rd());

	std::list<std::pair<int, int>> edges;

	int* powers = new int[versQuan];
	for (int i = 0; i < versQuan; i++) {
		powers[i] = 0;
	}

	for (int i = 0; i < versQuan; i++) {

		// Start vertex of the edge
		int start = i + 1;

		int edgeQuan = g() % maxPower + 1;

		for (int j = powers[i]; j < edgeQuan; j++) {

			// End vertex of the edge
			int finish;

			bool isEdgeCorrect = false;

			while (!isEdgeCorrect) {

				// Generating the end vertex of the edge
				finish = g() % versQuan + 1;

				// Checking if edge is correct
				if (start != finish) {

					isEdgeCorrect = true;

					// Checking all other edges
					std::list<std::pair<int, int>>::iterator it = edges.begin();

					while (it != edges.end()) {
						if ((start == it->first && finish == it->second) ||
							(start == it->second && finish == it->first)) {
							isEdgeCorrect = false;
							break;
						}
						++it;
					}
				}
			}

			// Adding the correct edge to the list
			edges.emplace_back(std::pair<int, int>(start, finish));

			// Incrementing the powers
			powers[start - 1]++;
			powers[finish - 1]++;
		}

	}

	// Opening the file
	FILE* file = fopen(fileName, "wt");

	// Outputting the characteristics of the graph
	fprintf(file, "%d %d\n", versQuan, edges.size());

	// Outputting the edges
	std::list<std::pair<int, int>>::iterator it = edges.begin();

	while (it != edges.end()) {
		fprintf(file, "%d %d\n", it->first, it->second);
		++it;
	}

	fclose(file);
}
