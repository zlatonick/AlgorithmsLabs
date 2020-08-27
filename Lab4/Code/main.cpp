#include "globals.h"
#include "Graph.h"
#include "CountryInitializer.h"

int main() {

	CountryInitializer initer;

	double** matrix = initer.getMatrix();

	Graph graph(matrix, CITY_QUAN);

	std::cout << "The adjacency matrix was created" << std::endl;

	// TEMP
	for (int i = 0; i < CITY_QUAN; i++) {
		for (int j = 0; j < CITY_QUAN; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	int* route = new int[CITY_QUAN];
	int routeLen = 0;

	/*int beginID = 0;
	int endID = 0;
	std::cout << "Enter the end vertex: ";
	std::cin >> endID;

	double* heuristic = initer.getHeuristic(endID);
	graph.initHeuristic(heuristic);

	std::cout << "The heuristic was calculated" << std::endl;

	// TEMP
	for (int i = 0; i < CITY_QUAN; i++) {
		std::cout << heuristic[i] << std::endl;
	}

	while (true) {
		std::cout << "Enter the begin vertex (-1 to exit): ";
		std::cin >> beginID;

		if (beginID == -1) {
			break;
		}

		delete[] route;
		route = new int[CITY_QUAN];
		routeLen = 0;

		double result = graph.findDistanceHeuristic(beginID, endID, route, routeLen);
		std::cout << "The results of greedy algorythm:" << std::endl;
		std::cout << "Length: " << result << std::endl;
		std::cout << "Route:";
		for (int i = 0; i < routeLen; i++) {
			std::cout << " " << route[i];
		}
		std::cout << std::endl;

		delete[] route;
		route = new int[CITY_QUAN];
		routeLen = 0;

		result = graph.findDistanceAStar(beginID, endID, route, routeLen);
		std::cout << "The results of A* algorythm:" << std::endl;
		std::cout << "Length: " << result << std::endl;
		std::cout << "Route:";
		for (int i = 0; i < routeLen; i++) {
			std::cout << " " << route[i];
		}
		std::cout << std::endl;

	}*/

	for (int i = 0; i < CITY_QUAN; i++) {
		double* heuristic = initer.getHeuristic(i);
		graph.initHeuristic(heuristic);

		for (int j = 0; j < CITY_QUAN; j++) {
			if (i < j) {
				std::cout << "Route from " << initer.cities[j] << " to " << initer.cities[i] << ":" << std::endl;

				delete[] route;
				route = new int[CITY_QUAN];
				routeLen = 0;

				double result = graph.findDistanceHeuristic(j, i, route, routeLen);
				std::cout << "The results of greedy algorythm:" << std::endl;
				std::cout << "Length: " << result << std::endl;
				std::cout << "Route:";
				for (int i = 0; i < routeLen; i++) {
					std::cout << " " << initer.cities[route[i]];
				}
				std::cout << std::endl;

				delete[] route;
				route = new int[CITY_QUAN];
				routeLen = 0;

				result = graph.findDistanceAStar(j, i, route, routeLen);
				std::cout << "The results of A* algorythm:" << std::endl;
				std::cout << "Length: " << result << std::endl;
				std::cout << "Route:";
				for (int i = 0; i < routeLen; i++) {
					std::cout << " " << initer.cities[route[i]];
				}
				std::cout << "\n" << std::endl;
			}
		}

		delete[] heuristic;
	}

	system("pause");
}