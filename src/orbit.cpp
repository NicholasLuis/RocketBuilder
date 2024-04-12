# include <../src/orbit.h>

Orbit::Orbit() // default constructor
{
	// Kennedy Space Center is the default launch site
	launchCoords.push_back(28.5744); // Lattitude ( >0 for northern hemisphere and <0 for southern hemisphere)
	launchCoords.push_back(80.6520); // Longitude
	initialVelo *= cos(launchCoords[0]); // Adjusts initial velocity depending on the lattitude
	toPrint = "Nothing to print yet";
	print(toPrint);

}
void Orbit::launchPossibilities(double deltaV) // Possible orbit radii depending on available delta V
{
	double finalVelo = initialVelo + deltaV;
	double finalPos = sqrt((finalVelo * finalVelo) / MU);
	toPrint = "There is enough delta V to get to an altitude of " + std::to_string( finalPos - initialPos);
	print(toPrint);
}
void Orbit::inclinationPossibilities() // Possible launch inclinations from earth
{
	if (launchCoords[0] > 0) // If the rocket is launching from the northern hemisphere
	{
		toPrint = "The rocket can launch into inclinations between " + std::to_string(launchCoords[1]) +
				  " degrees and 90 degrees";
	}
	else if (launchCoords[0] < 0) // If the rocket is launching from the souther hemisphere
	{
		toPrint = "The rocket can launch into inclinations between " + std::to_string(launchCoords[1]) +
				  " degrees and -90 degrees";
	}
	else // The rocket is launching from the equator
	{
		toPrint = "The rocket can only launch into an inclination of 0 degrees or 180 degrees";
	}
	print(toPrint);
}

void Orbit::inclinationPossibilities(double deltaV, double velocity) // must enter velocity as it crosses the equitorial plane
{
	double delta_i = 2.0 * asin(0.5 * deltaV / velocity) * (180/PI);
	toPrint = "The satellite can change its inclination by " + std::to_string(delta_i) + " degrees";
}

void Orbit::inclinationPossibilities(double deltaV, double radius, int e) // radius assuming circular orbit
{
	if (e != 0)
	{
		toPrint = "The eccentricity must be 0. Solution may not be accurate otherwise";
	}
	else
	{
		double velocity = sqrt(MU / (radius * radius));
		double delta_i = 2.0 * asin(0.5 * deltaV / velocity) * (180 / PI);
		toPrint = "The satellite can change its inclination by " + std::to_string(delta_i) + " degrees";
	}
	print(toPrint);
}

void Orbit::print(const std::string& str) {
	std::cout << str <<std::endl;
	//std::lock_guard<std::mutex> lock(console_mtx);
	//ConsoleManager::getInstance().log(str);
}