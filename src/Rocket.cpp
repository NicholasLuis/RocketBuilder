#include "Rocket.h"

// -----TOTAL STAGE CLASS-----
RocketStage::RocketStage(double structW, double fuelW, double specImp)
{
	stageStructureMass = structW;
	stageFuelMass = fuelW;
	stageTotalMass = stageStructureMass + stageFuelMass;

	totalMass += stageTotalMass;

	I_sp = specImp;
}
RocketStage::~RocketStage()
{
	totalMass -= stageStructureMass; // Assumes that all the fuel has been spent before stage detaches
}
double RocketStage::getFuelMass()
{
	return stageFuelMass;
}
double RocketStage::getStructureMass()
{
	return stageStructureMass;
}
double RocketStage::getTotalMass()
{
	return stageTotalMass;
}
double RocketStage::getI_sp() 
{
	return I_sp;
}

// -----TOTAL ROCKET CLASS-----
void TotalRocket::addToRocket(RocketStage* rocketPart2Add)
{
	totalRocketQueue.push(rocketPart2Add); // Adds a stage to the end
}
double TotalRocket::getDeltaV() // this calculates the delta V if you burn all the fuel from all of the remaing stages
{
	// makes a temporary copy of the RocketQueue and pops it so that it doesn't delete the rocket after doing the 
	std::queue<RocketStage*> copyOfRocketQueue = totalRocketQueue;
	double totalDeltaV = 0, stageDeltaV = 0;
	
	double stageISP, initialMass, finalMass, logMassRatio;

	for (int i = 0; i < copyOfRocketQueue.size(); i++)
	{
		stageISP = copyOfRocketQueue.front()->getI_sp();
		initialMass = copyOfRocketQueue.front()->getTotalMass();
		finalMass = copyOfRocketQueue.front()->getStructureMass(); // final mass of the stage is just the structure (all fuel burnt)
		logMassRatio = std::log(initialMass / finalMass);
		stageDeltaV = stageISP * g * logMassRatio;

		totalDeltaV += stageDeltaV;
		// Calculates the delta V of each stage
		copyOfRocketQueue.pop(); // Does not delete the objects inside since it only contains pointers
		
	}
}
// OPTIONAL FEATURE: COME BACK TO THIS ONCE WE'RE DONE WITH THE PROJECT
//double TotalRocket::getDeltaV( double fuelToBurn ) // this calculates the delta V if you only burn a certain amount of fuel
//{
//
//}
void TotalRocket::detatchStage()
{	
	totalRocketQueue.front()->~RocketStage();	// 1. Delete stage object (call de-constructor)
	totalRocketQueue.pop();						// 2. remove stage from the queue

}