#include "globals.h"
#include "Graph.h"

int main() {
	
	char* fileName = "graph.txt";

	const int versQuan = 100;
	const int maxPower = 20;

	// Generate the graph
	Graph::generateGraph(versQuan, maxPower, fileName);

	// Create the graph
	Graph* graph = new Graph(fileName);

	// Finding the minimal painting using the bees algorythm
	int* colours = graph->paintBees();

	// Outputting the results
	std::cout << "Quantity of colours " << graph->coloursQuan(colours) << std::endl;

	std::cout << "Colours:";
	for (int i = 0; i < graph->getVersQuan(); i++) {
		std::cout << " " << colours[i];
	}
	std::cout << std::endl;

	//graph->print();

	// Clearing the memory
	delete graph;
	delete[] colours;

	system("pause");
}