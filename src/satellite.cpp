// satellite.cpp
# include <../src/satellite.h>




Satellite::Satellite(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::string line;
    if (!std::getline(file, name)) { // First line for satellite name
        throw std::runtime_error("Invalid TLE format: missing name");
    }
    // Updated to use regex for line 1
    std::regex line1Regex(R"(^1\s+(\d{5})([A-Z])\s+(\d{2})(\d{3})([A-Z])\s+(\d{2})(\d+\.\d+)\s+([+-]?\.\d+)\s+([+-]?\d+)[Ee]?([+-]\d+|\s+0)\s+([+-]?\d{5})[Ee]?([+-]\d+)\s+(\d)\s+(\d{4}))");
    std::smatch matches;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Invalid TLE format: missing line 1");
    }
    if (std::regex_search(line, matches, line1Regex)) {
        satellite_number = std::stoi(matches[1].str());
        classification = matches[2].str()[0];
        launch_year = matches[3].str();
        launch_number = matches[4].str();
        launch_piece = matches[5].str();
        epoch_year = std::stod(matches[6].str()) + 2000; // Adjusted for full year
        epoch_day = std::stod(matches[7].str());
        mean_motion_derivative = std::stod(matches[8].str());
        mean_motion_second_derivative = std::stod("0." + matches[9].str()) * pow(10, -std::stoi(matches[10].str()));
        bstar_drag = std::stod("0." + matches[11].str()) * pow(10, -std::stoi(matches[12].str()));
        ephemeris_type = std::stoi(matches[13].str());
        element_set_number = std::stoi(matches[14].str());
    }
    else {
        throw std::runtime_error("Failed to parse line 1 of TLE");
    }

    // Updated to use regex for line 2

    std::regex line2Regex(R"(^2\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d{8})\s?(\d+))");
    if (!std::getline(file, line)) {
        throw std::runtime_error("Invalid TLE format: missing line 2");
    }
    if (std::regex_search(line, matches, line2Regex)) {
        // The satellite number is repeated, hence it's captured again but not used here
        inclination = std::stod(matches[2].str());
        raan = std::stod(matches[3].str());
        eccentricity = std::stod("0." + matches[4].str()); // Correctly process eccentricity
        argument_of_perigee = std::stod(matches[5].str());
        mean_anomaly = std::stod(matches[6].str());
        mean_motion = std::stod(matches[7].str());
        revolution_number_at_epoch = std::stoi(matches[8].str());
    }
    else {
        throw std::runtime_error("Failed to parse line 2 of TLE");
    }
}