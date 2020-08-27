#include "CountryInitializer.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                  // JSON library


CountryInitializer::CountryInitializer() {
	this->client = new http_client(U("https://maps.googleapis.com/"));

	this->cities = new std::string[CITY_QUAN];

	this->cities[0] = "tirana";
	this->cities[1] = "durres";
	this->cities[2] = "vlore";
	this->cities[3] = "elbasan";
	this->cities[4] = "shkoder";
	this->cities[5] = "korca";
	this->cities[6] = "fier";
	this->cities[7] = "berat";
	this->cities[8] = "ljushnja";
	this->cities[9] = "saranda";
	this->cities[10] = "kavaje";
	this->cities[11] = "pogradec";
	this->cities[12] = "girokastra";
	this->cities[13] = "kukes";
	this->cities[14] = "tepelena";
}


CountryInitializer::~CountryInitializer() {
	delete this->client;
	delete[] this->cities;
}


double CountryInitializer::getDistanceBetween(std::string city1, std::string city2) {

	uri_builder builder(U("/maps/api/distancematrix/json"));
	builder.append_query(U("origins"), city1.c_str());
	builder.append_query(U("destinations"), city2.c_str());
	builder.append_query(U("key"), this->DistKey.c_str());

	auto response = this->client->request(methods::GET, builder.to_string());

	auto res = response.get().extract_json().get();

	int resInt = res[U("rows")].as_array()[0]
		[U("elements")].as_array()[0]
		[U("distance")].as_object()
		[U("value")].as_integer();

	return ((double) resInt) / 1000.;
}


std::pair<double, double> CountryInitializer::getCoords(std::string city) {

	uri_builder builder(U("/maps/api/geocode/json"));
	builder.append_query(U("address"), city.c_str());
	builder.append_query(U("key"), this->GeoKey.c_str());
	auto response = this->client->request(methods::GET, builder.to_string());

	auto res = response.get().extract_json().get();

	auto resultCoords = res[U("results")].as_array()[0]
		[U("geometry")]
	[U("location")];

	double xCoord = resultCoords[U("lat")].as_double();
	double yCoord = resultCoords[U("lng")].as_double();

	return std::pair<double, double>(xCoord, yCoord);
}


double** CountryInitializer::getMatrix() {

	double** result = new double*[CITY_QUAN];
	for (int i = 0; i < CITY_QUAN; i++) {
		result[i] = new double[CITY_QUAN];

		for (int j = 0; j < CITY_QUAN; j++) {
			result[i][j] = -1;
		}
	}

	// Adjanced cities
	result[0][1] = 0;
	result[0][3] = 0;
	result[0][4] = 0;
	result[0][13] = 0;
	result[1][10] = 0;
	result[2][6] = 0;
	result[2][9] = 0;
	result[2][14] = 0;
	result[3][5] = 0;
	result[3][8] = 0;
	result[3][10] = 0;
	result[3][11] = 0;
	result[4][13] = 0;
	result[5][11] = 0;
	result[6][7] = 0;
	result[6][8] = 0;
	result[7][8] = 0;
	result[8][10] = 0;
	result[9][12] = 0;
	result[12][14] = 0;

	// Finding the distances
	for (int i = 0; i < CITY_QUAN; i++) {
		for (int j = 0; j < CITY_QUAN; j++) {
			if (result[i][j] == 0) {
				double dist = this->getDistanceBetween(
					this->cities[i], this->cities[j]);

				result[i][j] = dist;
				result[j][i] = dist;
			}
		}
	}

	return result;
}


double* CountryInitializer::getHeuristic(int cityID) {

	double* result = new double[CITY_QUAN];

	std::pair<double, double> coords1 = this->getCoords(this->cities[cityID]);

	for (int i = 0; i < CITY_QUAN; i++) {
		if (i != cityID) {
			std::pair<double, double> coords2 = this->getCoords(this->cities[i]);

			double diffX = fabs(coords1.first - coords2.first) * 3.1415926 / 180;
			double diffY = fabs(coords1.second - coords2.second) * 3.1415926 / 180;

			double a = sin(diffX / 2) * sin(diffX / 2) + cos(coords1.first * 3.1415926 / 180)
				* cos(coords2.first * 3.1415926 / 180) * sin(diffY / 2) * sin(diffY / 2);

			double c = 2 * atan2(sqrt(a), sqrt(1 - a)) * 6371;

			result[i] = c;
		}
		else {
			result[i] = 0;
		}
	}

	return result;
}