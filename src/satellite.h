// satellite.h
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <cstring>
#include <stdexcept>


class Satellite {
public:
    Satellite(const std::string& filePath);                                                     // Constructor

    std::string getName() const { return name; } 											    // Getter Name
    int getSatelliteNumber() const { return satellite_number; }                                 // Getter Satellite Number
    char getClassification() const { return classification; } 								    // Getter Classification
    std::string getLaunchYear() const { return launch_year; } 								    // Getter Launch Year
    std::string getLaunchNumber() const { return launch_number; } 							    // Getter Launch Number
    std::string getLaunchPiece() const { return launch_piece; } 							    // Getter Launch Piece
    double getEpochYear() const { return epoch_year; } 									        // Getter Epoch Year
    double getEpochDay() const { return epoch_day; } 										    // Getter Epoch Day
    double getMeanMotionDerivative() const { return mean_motion_derivative; } 				    // Getter Mean Motion Derivative
    double getMeanMotionSecondDerivative() const { return mean_motion_second_derivative; } 	    // Getter Mean Motion Second Derivative
    double getBstarDrag() const { return bstar_drag; } 									        // Getter Bstar Drag
    int getEphemerisType() const { return ephemeris_type; } 								    // Getter Ephemeris Type
    int getElementSetNumber() const { return element_set_number; } 							    // Getter Element Set Number
    int getChecksumLine1() const { return checksum_line1; } 								    // Getter Checksum Line 1
    double getInclination() const { return inclination; } 								        // Getter Inclination
    double getRAAN() const { return raan; } 											        // Getter RAAN
    double getEccentricity() const { return eccentricity; } 								    // Getter Eccentricity
    double getArgumentOfPerigee() const { return argument_of_perigee; } 						// Getter Argument of Perigee
    double getMeanAnomaly() const { return mean_anomaly; } 								        // Getter Mean Anomaly 
    double getMeanMotion() const { return mean_motion; } 								        // Getter Mean Motion
    int getRevolutionNumberAtEpoch() const { return revolution_number_at_epoch; } 			    // Getter Revolution Number at Epoch
    int getChecksumLine2() const { return checksum_line2; }  							        // Getter Checksum Line 2

private:
    std::string name;                               // Name of the satellite
    int satellite_number = 0; 					    // Satellite number
    char classification = ' '; 					    // Classification
    std::string launch_year; 					    // Launch year
    std::string launch_number; 					    // Launch number
    std::string launch_piece; 					    // Launch piece
    double epoch_year = 0.0; 					    // Epoch year
    double epoch_day = 0.0; 						// Epoch day
    double mean_motion_derivative = 0.0; 		    // Mean motion derivative
    double mean_motion_second_derivative = 0.0; 	// Mean motion second derivative
    double bstar_drag = 0.0; 					    // Bstar drag
    int ephemeris_type = 0; 					    // Ephemeris type
    int element_set_number = 0; 				    // Element set number
    int checksum_line1 = 0; 					    // Checksum Line 1
    double inclination = 0.0; 					    // Inclination
    double raan = 0.0; 						        // RAAN
    double eccentricity = 0.0; 					    // Eccentricity
    double argument_of_perigee = 0.0; 			    // Argument of Perigee
    double mean_anomaly = 0.0; 					    // Mean Anomaly
    double mean_motion = 0.0; 					    // Mean Motion
    int revolution_number_at_epoch = 0; 		    // Revolution Number at Epoch
    int checksum_line2 = 0; 					    // Checksum Line 2
};
