// Each part of the rocket has its own class

#ifndef Rocket_H
#define Rocket_H

#include <vector>
#include "global.h"

// Rocket Parts
class RocketParts // parent class: each rocket part should have these quantities
{
public:
	double getWeight(); // Returns the weight of the part
};

class engines : public RocketParts // the engines are one of the pieces of the rocket
{
private:
	double thrust;
	double structuralWeight;
	double fuelWeight;
	double I_sp; // specific impulse
public:
	engines(double Thrust, double StructWeight, double FuelWeight, double I_sp); // Default constructor for building unique part
};

class booster : public RocketParts
{
private:
	double thrust;
	double structuralWeight;
	double fuelWeight;
	double I_sp; // specific impulse
public:
	booster(double Thrust, double StructWeight, double FuelWeight, double I_sp); // Default constructor for building unique part
};

// Total Rocket
class TotalRocket 
{
private:
	std::vector<RocketParts*> TotalRocket; 
	// ^ A vector that pieces together the individual components of the rocket (Can also hold objects derived from RocketParts class)
	// Note: Build the rocket from top->bottom

public:

	double getFuelWeight(); // Returns the weight of the fuel
	double getTotalWeight(); // Returns the total weight of the rocket

	void addToRocket(RocketParts* rocketPart2Add); 
	void stageSeparation();
	void getDeltaV();
};

#endif