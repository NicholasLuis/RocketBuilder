#include "Rocket.h"

// -----ENGINES CLASS-----
//engines::engines(double Thrust, double StructWeight, double FuelWeight, double specImp)
//{
//	thrust = Thrust;
//	structuralWeight = StructWeight;
//	fuelWeight = FuelWeight;
//	I_sp = specImp;
//}
//double engines::getStructureWeight()
//{
//
//}

// -----TOTAL STAGE CLASS-----
RocketStage::RocketStage(double structW, double fuelW)
{
	stageStructureWeight = structW;
	stageFuelWeight = fuelW;
	stageTotalWeight = stageStructureWeight + stageFuelWeight;

	totalWeight += stageTotalWeight;
}
double RocketStage::getFuelWeight()
{
	return stageFuelWeight;
}

// -----TOTAL ROCKET CLASS-----
void TotalRocket::addToRocket(RocketStage* rocketPart2Add)
{
	totalRocketVector.insert(totalRocketVector.begin(), rocketPart2Add); // Adds a rocket stage to the total rocket
}

double TotalRocket::getDeltaV() // this calculates the delta V if you burn all the fuel
{
	double totalRocketWeight = 0;
	double totalFuelWeight = 0;
	for (int i = 0; i < totalRocketVector.size(); i++)
	{
		totalRocketWeight += totalRocketVector[i]->getStructureWeight();

		totalRocketWeight += totalRocketVector[i]->getFuelWeight();
		totalFuelWeight += totalRocketVector[i]->getFuelWeight();
	}

}