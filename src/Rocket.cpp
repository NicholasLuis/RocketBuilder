#include "Rocket.h"

// -------ROCKET CLASS--------
Rocket::Rocket() : fuelMass(0.0), structureMass(0.0), totalMass(0.0) {};
Rocket::~Rocket() {};

// -----TOTAL STAGE CLASS-----
RocketStage::RocketStage() {};
RocketStage::RocketStage(double structW = 0.0, double fuelW = 0.0, double specImp = 0.0)
	: stageStructureMass(structW), stageFuelMass(fuelW), I_sp(specImp), stageTotalMass(0.0) {
	updateTotalMass();
}
RocketStage::~RocketStage()
{
	Rocket::totalMass -= stageStructureMass; // Assumes that all the fuel has been spent before stage detaches
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

void RocketStage::setStructureMass(double mass) {
		stageStructureMass = mass;
		updateTotalMass();
}

void RocketStage::setFuelMass(double mass) {
	stageFuelMass = mass;
	updateTotalMass();
}

void RocketStage::setI_sp(double isp) { I_sp = isp; }

void RocketStage::updateTotalMass() {
	stageTotalMass = stageStructureMass + stageFuelMass;
}

void RocketStage::setMutex(std::mutex* mutex) {
	printMutex = mutex;
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
		stageDeltaV = stageISP * 9.80665 * logMassRatio;

		totalDeltaV += stageDeltaV;
		// Calculates the delta V of each stage
		copyOfRocketQueue.pop(); // Does not delete the objects inside since it only contains pointers

	}
	return totalDeltaV;
}
double TotalRocket::getDeltaV( double fuelToBurn ) // this calculates the delta V if you only burn a certain amount of fuel
{
	std::queue<RocketStage*> copyOfRocketQueue = totalRocketQueue;
	double totalDeltaV = 0, stageDeltaV = 0;

	double stageISP, initialMass, finalMass, logMassRatio, fuelMassTracker = 0;

	for (int i = 0; i < copyOfRocketQueue.size(); i++)
	{
		stageISP = copyOfRocketQueue.front()->getI_sp();
		initialMass = copyOfRocketQueue.front()->getTotalMass();
		finalMass = copyOfRocketQueue.front()->getStructureMass(); // final mass of the stage is just the structure (all fuel burnt)
		fuelMassTracker += finalMass - initialMass;
		if (fuelMassTracker > fuelToBurn) { // Checks if we're burning more fuel than what is asked
			// if true, it means that the stage has more fuel than what we need
			finalMass += (fuelMassTracker - fuelToBurn); // Adjusts the final weight because not all fuel is burnt
			logMassRatio = std::log(initialMass / finalMass);

			totalDeltaV += stageDeltaV;
			return totalDeltaV;
		}
		else
		{
			logMassRatio = std::log(initialMass / finalMass);
			stageDeltaV = stageISP * 9.80665 * logMassRatio;

			totalDeltaV += stageDeltaV;
		}
		// Calculates the delta V of each stage
		copyOfRocketQueue.pop(); // Does not delete the objects inside since it only contains pointers
	}
	return totalDeltaV;
}
void TotalRocket::detatchStage()
{
	totalRocketQueue.front()->~RocketStage();	// 1. Delete stage object (call de-constructor)
	totalRocketQueue.pop();						// 2. remove stage from the queue

}
void TotalRocket::setMutex(std::mutex* mutex)
{
	printMutex = mutex;
}

// Function to cleanly control the console_mtx and control the outputs to console
// rather than having the same two lines dirty up the code.
std::mutex Rocket::console_mtx;
void Rocket::log(const std::string& str) {
	std::lock_guard<std::mutex> lock(console_mtx);
	ConsoleManager::getInstance().log(str);
}