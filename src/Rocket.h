// Each part of the rocket has its own class

#ifndef Rocket_H
#define Rocket_H

#include <queue>
#include "global.h"
#include <cmath> // needed to take natural log for deltaV calculations
#include <mutex>

class Rocket // Each rocket stage share these quantities
{			// This class is only a template for other classes to use
protected:
	double totalMass; // total mass of the whole rocket
	double fuelMass; // total amount of fuel in the whole rocket
	double structureMass;
	std::mutex* printMutex = new std::mutex; // Mutex that controls print to console (a shared resource)

public: 
	virtual double getFuelMass() = 0;
	virtual double getStructureMss() = 0;
	virtual double getTotalMass() = 0;
	virtual void setMutex(std::mutex mutex);
};


class RocketStage : public Rocket // Each stage is a rocket in of itself
{
private: 
	double stageTotalMass; // this is the mass of the stage
	double stageFuelMass;
	double stageStructureMass;
	double I_sp;

public:
	RocketStage(double structW, double fuelW, double I_sp);
	~RocketStage(); // de-constructor
	double getFuelMass();
	double getStructureMass();
	double getTotalMass();
	double getI_sp();
	virtual void setMutex(std::mutex* mutex);
};


// Total Rocket
class TotalRocket : public Rocket // The total rocket is obviously also a rocket
{
private:
	std::queue<RocketStage*> totalRocketQueue; 
	// ^ A list that pieces together the individual components of the rocket (Can also hold objects derived from RocketParts class)
	// Note: Build the rocket from bottom->top

public:

	void addToRocket(RocketStage* rocketPart2Add); 
	void detatchStage(); // Detaches the bottom stage (obviously)
	double getDeltaV(); // Returns the delta v if you burn all the fuel
//	double getDeltaV(double fuelToBurn); // Returns the delta v if you burn all the fuel
	virtual void setMutex(std::mutex* mutex);

};

#endif