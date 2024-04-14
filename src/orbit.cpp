# include <../src/orbit.h>

# ifndef CONST
# define CONST
const double PI = 3.141592653;
const double MU = 3.986 * (10 ^ 5);// G * M_earth
const double R_Earth = 6370; // radius of the earth
# endif // CONST

// Log Function to Control Log Messages
void Orbit::log(const std::string& str) {
	ConsoleManager::getInstance().log(str);
} 
template<typename T>
void Orbit::log(const std::string& str, T& var) {
	ConsoleManager::getInstance().log(str, var);
}




Orbit::Orbit() : altitude(0.0), inclination(0.0), totalEnergy(0.0), lastCalculatedAltitude(0.0),
lastCalculatedVelocity(0.0), lastCalculatedInclination(0.0), isLaunched(false) // default constructor
{
	// Kennedy Space Center is the default launch site
	launchCoords.push_back(28.5744); // Lattitude ( >0 for northern hemisphere and <0 for southern hemisphere)
	launchCoords.push_back(80.6520); // Longitude
	initialVelo *= cos(launchCoords[0] * (PI / 180)); // Adjusts initial velocity depending on the lattitude
	initialPos = R_Earth;
}

Orbit::Orbit(double inputAltitude) : altitude(inputAltitude), inclination(0.0), totalEnergy(0.0), lastCalculatedAltitude(0.0),
lastCalculatedVelocity(0.0), lastCalculatedInclination(0.0), isLaunched(false) // parameterized constructor
{
	// Kennedy Space Center is the default launch site
	launchCoords.push_back(28.5744); // Lattitude ( >0 for northern hemisphere and <0 for southern hemisphere)
	launchCoords.push_back(80.6520); // Longitude
	initialVelo *= cos(launchCoords[0] * (PI / 180)); // Adjusts initial velocity depending on the lattitude
	initialPos = R_Earth + altitude;
}

Orbit::Orbit(double inputAltitude, double inputInclination) : altitude(inputAltitude), inclination(inputInclination), totalEnergy(0.0), lastCalculatedAltitude(0.0),
lastCalculatedVelocity(0.0), lastCalculatedInclination(0.0), isLaunched(false) // parameterized constructor
{
	// Kennedy Space Center is the default launch site
	launchCoords.push_back(28.5744); // Lattitude ( >0 for northern hemisphere and <0 for southern hemisphere)
	launchCoords.push_back(80.6520); // Longitude
	initialVelo *= cos(launchCoords[0] * (PI / 180)); // Adjusts initial velocity depending on the lattitude
	initialPos = R_Earth + altitude;
}
void Orbit::setRadius(double inputRadius)
{
	initialPos = inputRadius;
}
void Orbit::setCoords(double latitude, double longitude)
{
	launchCoords[0] = latitude;
	launchCoords[1] = longitude;
	initialVelo *= cos(launchCoords[0] * (PI / 180)); // Updates the initial velocity based on its current position
	std::ostringstream msg;
	msg << "initial velocity is " << initialVelo; 
	log(msg.str());
	msg.str("");
	msg.clear();
}
double Orbit::getRadius()
{
	return initialPos;
}

void Orbit::launchPossibilities(double deltaV) // Possible orbit radii depending on available delta V
{
	double finalVelo = initialVelo + deltaV;
	log("Final Velo will be " + std::to_string(finalVelo));
	double finalPos = sqrt((finalVelo * finalVelo) / MU);
	std::ostringstream msg;
	msg << "There is enough delta V to get to an altitude of " <<finalPos << " km";
	log(msg.str());
	msg.str("");
	msg.clear();
}
void Orbit::inclinationPossibilities() // Possible launch inclinations from earth
{
	std::ostringstream msg;
	if (launchCoords[0] > 0) // If the rocket is launching from the northern hemisphere
	{
		msg << "The rocket can launch into inclinations between " << launchCoords[0] <<
			" degrees and 90 degrees";
	}
	else if (launchCoords[0] < 0) // If the rocket is launching from the souther hemisphere
	{
		msg << "The rocket can launch into inclinations between " << launchCoords[0] <<
			" degrees and -90 degrees";
	}
	else // The rocket is launching from the equator
	{
		msg << "The rocket can only launch into an inclination of 0 degrees or 180 degrees";
	}
	log(msg.str());
	msg.str("");
	msg.clear();
}

void Orbit::inclinationPossibilities(double deltaV, double velocity) // must enter velocity as it crosses the equitorial plane
{
	std::ostringstream msg;
	double delta_i = 2.0 * asin(0.5 * deltaV / velocity) * (180 / PI);
	msg << "The satellite can change its inclination by " << delta_i << " degrees";
	log(msg.str());
	msg.str("");
	msg.clear();
}

void Orbit::inclinationPossibilities(double deltaV, double radius, int e) // radius assuming circular orbit
{
	std::ostringstream msg;
	if (e != 0)
	{
		msg << "The eccentricity must be 0. Solution may not be accurate otherwise";
	}
	else
	{
		double velocity = sqrt(MU / (radius * radius));
		double delta_i = 2.0 * asin(0.5 * deltaV / velocity) * (180 / PI);
		msg << "The satellite can change its inclination by " << delta_i << " degrees";
	}
	log(msg.str());
	msg.str("");
	msg.clear();
}

double Orbit::getCurrentAltitude() { return lastCalculatedAltitude; }
void Orbit::setAltitude(double inputAltitude) { altitude = inputAltitude; }
double Orbit::getCurrentVelocity() { return lastCalculatedVelocity; }
void Orbit::setVelocity(double inputVelocity) { initialVelo = inputVelocity; }
double Orbit::getCurrentInclination() { return lastCalculatedInclination; }
bool Orbit::getLaunchStatus() { return isLaunched; }

void Orbit::setLaunchStatus(bool status) { isLaunched = status; }
void Orbit::updateLastCalculatedValues(double altitude, double velocity, double inclination) {
	lastCalculatedAltitude = altitude;
	lastCalculatedVelocity = velocity;
	lastCalculatedInclination = inclination;
}

