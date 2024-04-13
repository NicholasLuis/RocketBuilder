# pragma once

class Orbit {
public:
    double altitude;                                // in kilometers
    double inclination;                             // in degrees
    Orbit() : altitude(0.0), inclination(0.0) {}    // Default constructor
};
