#pragma once
#include "globals.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                  // JSON library

class CountryInitializer {
	
	std::string DistKey = "AIzaSyAd9zxdwVZq5VIr2jGOmRmeUGfyOYgKlOo";
	std::string GeoKey = "AIzaSyCQMDDLBvN46svpPVIw415yrhqHerLoEAY";

	http_client* client;

	double getDistanceBetween(std::string city1, std::string city2);

	std::pair<double, double> getCoords(std::string city);

public:
	CountryInitializer();

	~CountryInitializer();

	std::string* cities;

	double** getMatrix();
	double* getHeuristic(int cityID);
};