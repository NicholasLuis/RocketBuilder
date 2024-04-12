# pragma once
#include <vector>
#include <string>
#include <cmath>
#include <mutex>
# include <../src/ConsoleManager.h>

const double PI = 3.141592653;
const double MU = 3.986 * (10 ^ 5);// G * M_earth

// This class controls the orbit stuff
// For example: given the initial position and deltaV, it will output possible orbits depending on user input
class Orbit { // Always assume perfectly circular orbit
private: 
	double initialVelo = 0.4651; // (km/s) Initial velocity = tangential velocity on equator
	std::vector<double> launchCoords; // Initializes a vector of size 2 (lat and long coordinates)
	std::string toPrint;
//	static std::mutex console_mtx;  // Console Mutex to control the shared resource internally

	double totalEnergy;
public:
	Orbit(); // Default constructor: assumes that the rocket is launching from earth

	static void print(const std::string& message);    // Log function for clean console output and control of console

	void launchPossibilities(double deltaV);
	void inclinationPossibilities(); // Launching from earth
	void inclinationPossibilities(double deltaV, double velocity); // Inclination change (already in orbit)
	void inclinationPossibilities(double deltaV, double radius, int e); // Inclination change (already in orbit)


	double altitude;
	double inclination;
};
