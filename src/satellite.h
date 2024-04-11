// satellite.h
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <cstring>

class Satellite {
public:
    Satellite(const std::string& filePath);

    void displayTLE() const;
    std::string getName() const { return name; }
    std::string getFormattedTLE() const;

    int getSatelliteNumber() const { return satellite_number; }
    char getClassification() const { return classification; }
    std::string getLaunchYear() const { return launch_year; }
    std::string getLaunchNumber() const { return launch_number; }
    std::string getLaunchPiece() const { return launch_piece; }
    double getEpochYear() const { return epoch_year; }
    double getEpochDay() const { return epoch_day; }
    double getMeanMotionDerivative() const { return mean_motion_derivative; }
    double getMeanMotionSecondDerivative() const { return mean_motion_second_derivative; }
    double getBstarDrag() const { return bstar_drag; }
    int getEphemerisType() const { return ephemeris_type; }
    int getElementSetNumber() const { return element_set_number; }
    int getChecksumLine1() const { return checksum_line1; }
    double getInclination() const { return inclination; }
    double getRAAN() const { return raan; }
    double getEccentricity() const { return eccentricity; }
    double getArgumentOfPerigee() const { return argument_of_perigee; }
    double getMeanAnomaly() const { return mean_anomaly; }
    double getMeanMotion() const { return mean_motion; }
    int getRevolutionNumberAtEpoch() const { return revolution_number_at_epoch; }
    int getChecksumLine2() const { return checksum_line2; }

private:
    std::string name;
    int satellite_number = 0;
    char classification = ' ';
    std::string launch_year;
    std::string launch_number;
    std::string launch_piece;
    double epoch_year = 0.0;
    double epoch_day = 0.0;
    double mean_motion_derivative = 0.0;
    double mean_motion_second_derivative = 0.0;
    double bstar_drag = 0.0;
    int ephemeris_type = 0;
    int element_set_number = 0;
    int checksum_line1 = 0;
    double inclination = 0.0;
    double raan = 0.0;
    double eccentricity = 0.0;
    double argument_of_perigee = 0.0;
    double mean_anomaly = 0.0;
    double mean_motion = 0.0;
    int revolution_number_at_epoch = 0;
    int checksum_line2 = 0;
};
