# pragma once
#include <vector>
#include <string>
#include <cmath>

const double PI = 3.141592653;
const double MU = 3.986 * (10 ^ 5);// G * M_earth

// This class controls the orbit stuff
// For example: given the initial position and deltaV, it will output possible orbits depending on user input
class Orbit { // Always assume perfectly circular orbit
private: 
	double initialVelo = ; // Initial velocity = tangential velocity on equator
	std::vector<double> launchCoords; // Initializes a vector of size 2 (lat and long coordinates)
	std::string toPrint;
public:
	Orbit(); // Default constructor: assumes that the rocket is launching from earth

	void launchPossibilities(double deltaV);
	void inclinationPossibilities(); // Launching from earth
	void inclinationPossibilities(double deltaV, double velocity); // Inclination change (already in orbit)
	void inclinationPossibilities(double deltaV, double radius); // Inclination change (already in orbit)

};
