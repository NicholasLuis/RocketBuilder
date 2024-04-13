# pragma once
#include <vector>
#include <string>
#include <cmath>
#include <mutex>
# include <../src/global.h>

// This class controls the orbit stuff
// For example: given the initial position and deltaV, it will output possible orbits depending on user input
class Orbit { // Always assume perfectly circular orbit
private:
	double initialVelo = 0.4651; // (km/s) Initial velocity = tangential velocity on equator
	std::vector<double> launchCoords; // Initializes a vector of size 2 (lat and long coordinates)
	std::string toPrint;
	double initialPos;
	
	static void log(const std::string& message);				// Log function for clean console output and control of console
	template<typename T>
	void log(const std::string& str, T& var);

	double totalEnergy;
public:
	Orbit(); // Default constructor: assumes that the rocket is launching from earth
	Orbit(double inputAltitude); // Constructor for when the rocket is already in orbit
	Orbit(double inputAltitude, double inputInclination); // Constructor for when the rocket is already in orbit
	void setRadius(double inputRadius);
	void setCoords(double latitude, double longitude);
	double getRadius();

	void launchPossibilities(double deltaV);
	void inclinationPossibilities(); // Launching from earth
	void inclinationPossibilities(double deltaV, double velocity); // Inclination change (already in orbit)
	void inclinationPossibilities(double deltaV, double radius, int e); // Inclination change (already in orbit)

	double altitude;
	double inclination;
};