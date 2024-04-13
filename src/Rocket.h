// Each part of the rocket has its own class

# ifndef Rocket_H
# define Rocket_H

# include <queue>
# include <../src/global.h>
# include <cmath>
# include <mutex>

class Rocket													// Each rocket stage share these quantities
{																// This class is a template for other classes to use
protected:
	double totalMass;											// total mass of the whole rocket
	double fuelMass;											// total amount of fuel in the whole rocket
	double structureMass;										// total mass of the structure of the whole rocket
public:
	Rocket();
	virtual ~Rocket();
	virtual double getFuelMass() = 0;							// mass of the fuel in the rocket
	virtual double getStructureMass() = 0;						// mass of the structure of the rocket
	virtual double getTotalMass() = 0;							// total mass of the rocket
	static void log(const std::string& message);				// Log function for clean console output and control of console
	template<typename T>
	void log(const std::string& str, T& var);
};


class RocketStage : public Rocket								// Each stage is a rocket in of itself
{
private:
	double stageTotalMass;										// this is the mass of the stage
	double stageFuelMass;										// this is the mass of the fuel in the stage
	double stageStructureMass;									// this is the mass of the structure of the stage
	double I_sp;												// specific impulse of the stage

public:
	RocketStage();
	RocketStage(double structW, double fuelW, double specImp);
	RocketStage(double structW, double fuelW, double specImp, double totMass);

	virtual ~RocketStage() override;							// de-constructor
	double getFuelMass() override;								// Corrected typo from 'getFuleMass' to 'getFuelMass'
	double getStructureMass() override;							// Corrected typo from 'getStrucutreMass' to 'getStructureMass'
	double getTotalMass() override;								// total mass of the stage
	double getI_sp();											// Corrected typo from 'getI_sp' to 'getI_sp'

	// Setters
	void setStructureMass(double mass);							// Corrected typo from 'setStrucutreMass' to 'setStructureMass'
	void setFuelMass(double mass);								// Corrected typo from 'setFuleMass' to 'setFuelMass'
	void setI_sp(double isp);									// Corrected typo from 'setI_sp' to 'setI_sp'

	void updateTotalMass();										// update the total mass of the stage
};


// Total Rocket
class TotalRocket : public Rocket								// The total rocket is obviously also a rocket
{
private:
	std::queue<std::shared_ptr<RocketStage>> totalRocketQueue;  // Queue managing rocket stages
	RocketStage* payload = nullptr;								// Optional payload stage
	std::string name; 											// Name of the rocket

public:
	TotalRocket(); 												// Constructor declaration
	virtual ~TotalRocket() override;							// Destructor declaration

	std::string getName();										// Getter for name
	void setName(std::string name); 							// Setter for name

	double getFuelMass(); 										// Getter for fuel mass
	double getStructureMass(); 									// Getter for structure mass
	double getTotalMass(); 										// Getter for total mass

	void setPayload(RocketStage* payloadStage); 				// Set payload stage
	void addToRocket(RocketStage* rocketPartToAdd); 			// Add rocket stage to the total rocket
	void detachStage(); // Corrected typo from 'detatchStage' to 'detachStage'
	double getDeltaV(); 										// Get delta V
	double getDeltaV(double fuelToBurn); 						// Get delta V with fuel to burn
	std::queue<std::shared_ptr<RocketStage>> getStageQueue(); 	// Get stage queue
};

# endif