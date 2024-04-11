// Each part of the rocket has its own class

#ifndef Rocket_H
#define Rocket_H

#include <vector>
#include "global.h"


class Rocket // Each rocket stage share these quantities
{			// This class is only a template for other classes to use
protected:
	double totalWeight; // total weight of the rocket
	double fuelWeight; // total amount of fuel in the rocket
	double structureWeight;

	virtual double getTotalWeight();
	virtual double getFuelWeight();
	virtual double getStructureWeight();
};


class RocketStage : public Rocket // Each stage is a rocket in of itself
{
private: 
	double stageTotalWeight; // this is the weight of the stage
	double stageFuelWeight;
	double stageStructureWeight;

public:
	RocketStage(double structW, double fuelW);
	double getFuelWeight();
	double getTotalWeight();
};


// Total Rocket
class TotalRocket : public Rocket // The total rocket is obviously also a rocket
{
private:
	std::vector<RocketStage*> totalRocketVector; 
	// ^ A vector that pieces together the individual components of the rocket (Can also hold objects derived from RocketParts class)
	// Note: Build the rocket from top->bottom

public:

	double getFuelWeight(); // Returns the weight of the fuel
	double getTotalWeight(); // Returns the total weight of the rocket

	void addToRocket(RocketStage* rocketPart2Add); 
	double getDeltaV(); // Returns the delta v if you burn all the fuell
};

//// Rocket Parts
//class RocketParts // parent class: each rocket part should have these quantities
//{
//public:
//	virtual double getStructureWeight(); // Returns the weight of the part
//	virtual double getFuelWeight();
//};
//
//class engines : public RocketParts // the engines are one of the pieces of the rocket
//{
//private:
//	double thrust;
//	double structuralWeight;
//	double fuelWeight;
//	double I_sp; // specific impulse
//public:
//	engines(double Thrust, double StructWeight, double FuelWeight, double specImp); // Default constructor for building unique part
//	double getFuelWeight();
//	double getStructureWeight();
//};
//
//class booster : public RocketParts
//{
//private:
//	double thrust;
//	double structuralWeight;
//	double fuelWeight;
//	double I_sp; // specific impulse
//public:
//	booster(double Thrust, double StructWeight, double FuelWeight, double I_sp); // Default constructor for building unique part
//};
//
//// Total Rocket
//class TotalRocket 
//{
//private:
//	std::vector<RocketParts*> totalRocketVector; 
//	// ^ A vector that pieces together the individual components of the rocket (Can also hold objects derived from RocketParts class)
//	// Note: Build the rocket from top->bottom
//
//public:
//
//	double getFuelWeight(); // Returns the weight of the fuel
//	double getTotalWeight(); // Returns the total weight of the rocket
//
//	void addToRocket(RocketParts* rocketPart2Add); 
//	double getDeltaV(); // Returns the delta v if you burn all the fuell
//};

#endif