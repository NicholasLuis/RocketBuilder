// Each part of the rocket has its own class

#ifndef Rocket_H
#define Rocket_H

#include <vector>

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
	double weight;
public:

};
class booster : public RocketParts
{

};

// Total Rocket
class TotalRocket 
{
private:
	std::vector<RocketParts*> RocketPieces; // A vector that pieces together the individual components of the rocket (Can also hold objects derived from RocketParts class)
	
	engines* engine = new engines;


public:
	double getFuelWeight(); // Returns the weight of the fuel
	double getTotalWeight(); // Returns the total weight of the rocket

	void addEngine(engines* engine); 
};

#endif