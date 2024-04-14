#include "Rocket.h"

// Log Function to Control Log Messages
void Rocket::log(const std::string& str) {
	ConsoleManager::getInstance().log(str);
}
template<typename T>
void Rocket::log(const std::string& str, T& var) {
	ConsoleManager::getInstance().log(str, var);
}

// -------ROCKET (PARENT) CLASS--------
Rocket::Rocket() : fuelMass(0.0), structureMass(0.0), totalMass(0.0) {};		// Constructor
Rocket::~Rocket() {};															// Destructor

// -----ROCKET STAGE CLASS-----
//RocketStage::RocketStage() {};
RocketStage::RocketStage(double structW, double fuelW, double specImp)
	: stageStructureMass(structW), stageFuelMass(fuelW), I_sp(specImp), stageTotalMass(structW + fuelW) {
}
RocketStage::RocketStage(double structW = 0.0, double fuelW = 0.0, double specImp = 0.0, double totMass = 0.0)		// Constructor
	: Rocket(), stageStructureMass(structW), stageFuelMass(fuelW), I_sp(specImp), stageTotalMass(totMass) {			// Initializer list
	updateTotalMass();																								// Update the total mass
}

// Destructor
RocketStage::~RocketStage()
{
	Rocket::totalMass -= stageStructureMass;		// Assumes that all the fuel has been spent before stage detaches
	Rocket::totalMass -= stageFuelMass;				// Assumes that all the fuel has been spent before stage detaches
	Rocket::fuelMass -= stageFuelMass;				// Assumes that all the fuel has been spent before stage detaches
	Rocket::structureMass -= stageStructureMass;	// Assumes that all the fuel has been spent before stage 
}

// Getter Fuel Mass
double RocketStage::getFuelMass() {return stageFuelMass;}

// Getter Structure Mass
double RocketStage::getStructureMass() {return stageStructureMass;}

// Getter Total Mass
double RocketStage::getTotalMass(){return stageTotalMass;}

// Getter Specific Impulse
double RocketStage::getI_sp() { return I_sp; }

// Setter for structure mass
void RocketStage::setStructureMass(double mass) {
		stageStructureMass = mass;
		updateTotalMass();
}

// Setter for fuel mass
void RocketStage::setFuelMass(double mass) {
	stageFuelMass = mass;
	updateTotalMass();
}

// Setter for specific impulse
void RocketStage::setI_sp(double isp) { I_sp = isp; }

// Update the total mass of the stage
void RocketStage::updateTotalMass() { 
	stageTotalMass = stageStructureMass + stageFuelMass;
}


// -----TOTAL ROCKET CLASS-----
TotalRocket::TotalRocket() : Rocket(), payload(nullptr), name(" ") {}
TotalRocket::~TotalRocket() {
	// Deallocate all stages in the queue
	name = " ";												// Clear the name
	while (!totalRocketQueue.empty()) {						// While the queue is not empty
		totalRocketQueue.pop();								// Remove the front element from the queue
	}
	// Optionally, deallocate the payload if it exists
	if (payload) {
		delete payload;										// Ensure we do not leak memory if setting a new payload
		payload = nullptr; 									// Set the pointer to null
	}
}

// Getter for name
std::string TotalRocket::getName() { return name; }			// Getter for name

// Setter for name
void TotalRocket::setName(std::string inputName) { this->name = inputName; } // Setter for name

// Get the total fuel mass of the rocket
double TotalRocket::getFuelMass() {
	double totalFuelMass = 0.0;									// variable to store the total fuel mass
	auto tempQueue = totalRocketQueue; 							// temporary queue to store the stages

	while (!tempQueue.empty()) { 								// while the queue is not empty
		RocketStage* stage = tempQueue.front().get(); 			// get the front element
		totalFuelMass += stage->getFuelMass(); 					// add the fuel mass of the stage to the total fuel mass
		tempQueue.pop(); 										// remove the front element
	}

	return totalFuelMass; 										// return the total fuel mass
}

// Get the total structure mass of the rocket
double TotalRocket::getStructureMass() {
	double totalStructureMass = 0.0; 							// variable to store the total structure mass
	auto tempQueue = totalRocketQueue; 							// temporary queue to store the stages

	while (!tempQueue.empty()) { 								// while the queue is not empty
		RocketStage* stage = tempQueue.front().get(); 			// get the front element
		totalStructureMass += stage->getStructureMass();		// add the structure mass of the stage to the total structure mass
		tempQueue.pop(); 										// remove the front element
	}

	return totalStructureMass; 									// return the total structure mass
}

// Get the total mass of the rocket
double TotalRocket::getTotalMass() {
	double combinedMass = 0.0; 									// variable to store the total mass
	auto tempQueue = totalRocketQueue; 							// temporary queue to store the stages

	while (!tempQueue.empty()) { 								// while the queue is not empty
		RocketStage* stage = tempQueue.front().get();			// get the front element
		combinedMass += stage->getTotalMass(); 					// add the total mass of the stage to the total mass
		tempQueue.pop(); 										// remove the front element
	}

	return combinedMass; 										// return the total mass
}


// Set payload stage
void TotalRocket::setPayload(RocketStage* payloadStage) {	// Set payload stage
	if (payload) {
		delete payload;										// Ensure we do not leak memory if setting a new payload
	}
	payload = payloadStage;									// Set the payload stage
}

// Adds a stage to the end of the rocket
void TotalRocket::addToRocket(RocketStage* rocketPart2Add)
{
	totalRocketQueue.push(std::shared_ptr<RocketStage> (rocketPart2Add));	// Adds a stage to the end
}

// this calculates the delta V if you burn all the fuel from all of the remaing stages
double TotalRocket::getDeltaV() // this calculates the delta V if you burn all the fuel from all of the remaing stages
{
	// makes a temporary copy of the RocketQueue and pops it so that it doesn't delete the rocket after doing the 
	double totalDeltaV = 0, stageDeltaV = 0;				// variables to store the total delta V and the delta V of each stage

	double stageISP, initialMass, finalMass, logMassRatio;	// variables to store the values of the stage
	auto tempQueue = totalRocketQueue;						// temporary queue to store the stages

	for (int i = 0; i < tempQueue.size(); i++)	
	{
		stageISP = tempQueue.front()->getI_sp();			// ISP of the stage
		initialMass = TotalRocket::getTotalMass();	// initial mass of the stage is the total mass
		finalMass = initialMass - TotalRocket::getFuelMass();	// final mass of the stage is just the structure (all fuel burnt)
		logMassRatio = std::log(initialMass / finalMass);	// calculates the log of the mass ratio
		stageDeltaV = stageISP * 9.80665 * logMassRatio / 1000; 	// calculates the delta V of the stage

		totalDeltaV += stageDeltaV; 						// adds the delta V of the stage to the total delta V
		// Calculates the delta V of each stage
		tempQueue.pop(); // Does not delete the objects inside since it only contains pointers

	}
	return totalDeltaV; // returns the total delta V
}

// this calculates the delta V if you only burn a certain amount of fuel
double TotalRocket::getDeltaV( double fuelToBurn )			// this calculates the delta V if you only burn a certain amount of fuel
{
	double totalDeltaV = 0, stageDeltaV = 0;				// variables to store the total delta V and the delta V of each stage

	double stageISP, initialMass, finalMass, logMassRatio, fuelMassTracker = 0; // variables to store the values of the stage

	auto tempQueue = totalRocketQueue;						// temporary queue to store the stages

	for (int i = 0; i < tempQueue.size(); i++)
	{
		stageISP = tempQueue.front()->getI_sp(); 			// ISP of the stage	
		initialMass = TotalRocket::getTotalMass();	// initial mass of the stage is the total mass
		finalMass = initialMass - TotalRocket::getFuelMass();	// final mass of the stage is just the structure (all fuel burnt)
		fuelMassTracker += finalMass - initialMass; 		// keeps track of the fuel mass
		if (fuelMassTracker > fuelToBurn) {					// Checks if we're burning more fuel than what is asked
			// if true, it means that the stage has more fuel than what we need
			finalMass += (fuelMassTracker - fuelToBurn);		// Adjusts the final weight because not all fuel is burnt
			logMassRatio = std::log(initialMass / finalMass);	// calculates the log of the mass ratio
			stageDeltaV = stageISP * 9.80665 * logMassRatio;	// calculates the delta V of the stage
			totalDeltaV += stageDeltaV;
			return totalDeltaV;
		}
		else
		{
			logMassRatio = std::log(initialMass / finalMass);	// calculates the log of the mass ratio
			stageDeltaV = stageISP * 9.80665 * logMassRatio; 	// calculates the delta V of the stage

			totalDeltaV += stageDeltaV; 
		}
		// Calculates the delta V of each stage
		tempQueue.pop();										// Does not delete the objects inside since it only contains pointers
	}
	return totalDeltaV;											// returns the total delta V
}
void TotalRocket::detachStage()
{
	totalRocketQueue.pop();										// remove stage from the queue
}

// Get the queue of stages
std::queue<std::shared_ptr<RocketStage>> TotalRocket::getStageQueue() {
	return totalRocketQueue;									// returns the queue of stages
}
