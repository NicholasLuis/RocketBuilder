#include "Rocket.h"

// -----TOTAL ROCKET CLASS-----
void TotalRocket::addToRocket(RocketParts* rocketPart2Add)
{
	TotalRocket.insert(TotalRocket.begin(), rocketPart2Add); // Adds a rocket part to the total rocket
}

void getDeltaV