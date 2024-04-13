#include "Rocket.h"

// -------ROCKET CLASS--------
Rocket::Rocket() : fuelMass(0.0), structureMass(0.0), totalMass(0.0) {};
Rocket::~Rocket() {};

// -----TOTAL STAGE CLASS-----
template<typename T>
RocketStage<T>::RocketStage() {};
template<typename T>
RocketStage<T>::RocketStage(double structW, double fuelW, double specImp)
	: stageStructureMass(structW), stageFuelMass(fuelW), I_sp(specImp), stageTotalMass(0.0) {
	updateTotalMass();
}
template<typename T>
RocketStage<T>::~RocketStage()
{
	Rocket::totalMass -= stageStructureMass; // Assumes that all the fuel has been spent before stage detaches
}
template<typename T>
double RocketStage<T>::getFuelMass()
{
	return stageFuelMass;
}
template<typename T>
double RocketStage<T>::getStructureMass()
{
	return stageStructureMass;
}
template<typename T>
double RocketStage<T>::getTotalMass()
{
	return stageTotalMass;
}
template<typename T>
double RocketStage<T>::getI_sp()
{
	return I_sp;
}

template<typename T>
void RocketStage<T>::setStructureMass(double mass) {
	stageStructureMass = mass;
	updateTotalMass();
}

template<typename T>
void RocketStage<T>::setFuelMass(double mass) {
	stageFuelMass = mass;
	updateTotalMass();
}

template<typename T>
void RocketStage<T>::setI_sp(double isp) { I_sp = isp; }

template<typename T>
void RocketStage<T>::updateTotalMass() {
	stageTotalMass = stageStructureMass + stageFuelMass;
}



// -----TOTAL ROCKET CLASS-----
template<typename T>
TotalRocket<T>::TotalRocket() : payload(nullptr) {}

template<typename T>
TotalRocket<T>::~TotalRocket() {
	// Deallocate all stages in the queue
	while (!totalRocketQueue.empty()) {
		delete totalRocketQueue.front();  // Delete the object pointed to by the front pointer
		totalRocketQueue.pop();           // Remove the front element from the queue
	}
	// Optionally, deallocate the payload if it exists
	if (payload) {
		delete payload;
		payload = nullptr;
	}
}

template<typename T>
void TotalRocket<T>::setPayload(T* payloadStage) {
	if (payload) {
		delete payload;  // Ensure we do not leak memory if setting a new payload
	}
	payload = payloadStage;
}

template<typename T>
void TotalRocket<T>::addToRocket(T* rocketPart2Add)
{
	totalRocketQueue.push(rocketPart2Add); // Adds a stage to the end
}

template<typename T>
double TotalRocket<T>::getDeltaV() // this calculates the delta V if you burn all the fuel from all of the remaing stages
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

template<typename T>
double TotalRocket<T>::getDeltaV(double fuelToBurn) // this calculates the delta V if you only burn a certain amount of fuel
{
	std::queue<T*> copyOfRocketQueue = totalRocketQueue;
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
template<typename T>
void TotalRocket<T>::detachStage()
{
	if (!totalRocketQueue.empty()) {
		delete totalRocketQueue.front();// 1. Delete stage object (call de-constructor)
		totalRocketQueue.pop();
	}// 2. remove stage from the queue
					

}
template <typename T>
std::queue<T*> TotalRocket<T>::getStageQueue() const {
	return totalRocketQueue;
}

template<typename T>
double TotalRocket<T>::getFuelMass() {
	double totalFuelMass = 0.0;
	std::queue<T*> tempQueue = totalRocketQueue; // Copy the original queue

	while (!tempQueue.empty()) {
		T* stage = tempQueue.front();
		totalFuelMass += stage->getFuelMass();
		tempQueue.pop();
	}

	return totalFuelMass;
}

template<typename T>
double TotalRocket<T>::getStructureMass() {
	double totalStructureMass = 0.0;
	std::queue<T*> tempQueue = totalRocketQueue; // Copy the original queue

	while (!tempQueue.empty()) {
		T* stage = tempQueue.front();
		totalStructureMass += stage->getStructureMass();
		tempQueue.pop();
	}

	return totalStructureMass;
}

template<typename T>
double TotalRocket<T>::getTotalMass() {
	double combinedMass = 0.0;
	std::queue<T*> tempQueue = totalRocketQueue; // Copy the original queue

	while (!tempQueue.empty()) {
		T* stage = tempQueue.front();
		combinedMass += stage->getTotalMass();
		tempQueue.pop();
	}

	return combinedMass;
}




// Function to cleanly control the console_mtx and control the outputs to console
// rather than having the same two lines dirty up the code.
std::mutex Rocket::console_mtx;
void Rocket::log(const std::string& str) {
	std::lock_guard<std::mutex> lock(console_mtx);
	ConsoleManager::getInstance().log(str);
}