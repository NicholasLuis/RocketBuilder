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
double RocketStage::getStructureWeight()
{
	return stageStructureWeight;
}
double RocketStage::getTotalWeight()
{
	return stageTotalWeight;
}

// -----TOTAL ROCKET CLASS-----
void TotalRocket::addToRocket(RocketStage* rocketPart2Add)
{
	totalRocketVector.push(rocketPart2Add); // Adds a stage to the end
}
double TotalRocket::getDeltaV() // this calculates the delta V if you burn all the fuel
{
	
}
void TotalRocket::detatchStage()
{
	// 1. remove stage from the vector using .erase( )
	// 2. 
}