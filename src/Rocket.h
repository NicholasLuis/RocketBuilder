// Each part of the rocket has its own class

#ifndef Rocket_H
#define Rocket_H

#include <queue>
#include <../src/global.h>
#include <cmath> // needed to take natural log for deltaV calculations
#include <mutex>

class Rocket // Each rocket stage share these quantities
{			// This class is only a template for other classes to use
protected:
	double totalMass; // total mass of the whole rocket
	double fuelMass; // total amount of fuel in the whole rocket
	double structureMass;
	static std::mutex console_mtx;  // Console Mutex to control the shared resource internally

public:
	Rocket();
	~Rocket();
	virtual double getFuelMass() = 0;
	virtual double getStructureMass() = 0;
	virtual double getTotalMass() = 0;
	static void log(const std::string& message);    // Log function for clean console output and control of console
};


class RocketStage : public Rocket // Each stage is a rocket in of itself
{
private:
	double stageTotalMass; // this is the mass of the stage
	double stageFuelMass;
	double stageStructureMass;
	double I_sp;

public:
	RocketStage();
	RocketStage(double structW, double fuelW, double specImp);

	~RocketStage(); // de-constructor
	double getFuelMass() override;
	double getStructureMass() override;
	double getTotalMass() override;
	double getI_sp();

	// Setters
	void setStructureMass(double mass);
	void setFuelMass(double mass);
	void setI_sp(double isp);

	// Helper function to update the total mass of the stage
	void updateTotalMass();
};


// Total Rocket
class TotalRocket : public Rocket // The total rocket is obviously also a rocket
{
private:
	std::queue<RocketStage*> totalRocketQueue;  // Queue managing rocket stages
	RocketStage* payload = nullptr;  // Optional payload stage

public:
	TotalRocket();
	~TotalRocket();  // Destructor declaration

	double getFuelMass();
	double getStructureMass();
	double getTotalMass();
	void addToRocket(RocketStage* rocketPartToAdd);
	void detachStage(); // Corrected typo from 'detatchStage' to 'detachStage'
	void setPayload(RocketStage* payloadStage);
	double getDeltaV();
	double getDeltaV(double fuelToBurn);
	std::queue<RocketStage*> getStageQueue() const;
};

#endif