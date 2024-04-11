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

/*

Satellite::Satellite(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::string line;
    std::getline(file, name); // First line for satellite name

    if (!std::getline(file, line)) {
        throw std::runtime_error("Invalid TLE format: missing line 1");
    }
    std::istringstream iss1(line);
    iss1 >> satellite_number >> classification >> launch_year >> launch_number >> launch_piece
        >> epoch_year >> epoch_day >> mean_motion_derivative
        >> mean_motion_second_derivative >> bstar_drag >> ephemeris_type
        >> element_set_number >> checksum_line1;

    if (!std::getline(file, line)) {
        throw std::runtime_error("Invalid TLE format: missing line 2");
    }
    std::istringstream iss2(line);
    iss2 >> inclination >> raan >> eccentricity >> argument_of_perigee
        >> mean_anomaly >> mean_motion >> revolution_number_at_epoch >> checksum_line2;

    epoch_year += 2000; 
    eccentricity *= 1e-7; 
}


void Satellite::displayTLE() const {
    std::cout << "TLE for " << name << ":" << std::endl;
    std::cout << getFormattedTLE() << std::endl;

    std::cout << "Detailed Information:" << std::endl;
    std::cout << "Satellite Number: " << satellite_number << std::endl;
    std::cout << "Classification: " << classification << std::endl;
    std::cout << "Launch Year: " << launch_year << std::endl;
    std::cout << "Launch Number: " << launch_number << std::endl;
    std::cout << "Piece: " << launch_piece << std::endl;
    std::cout << "Epoch Year: " << epoch_year << std::endl;
    std::cout << "Epoch Day: " << epoch_day << std::endl;
    std::cout << "Inclination: " << inclination << " degrees" << std::endl;
    std::cout << "RAAN: " << raan << " degrees" << std::endl;
    std::cout << "Eccentricity: " << eccentricity << std::endl;
    std::cout << "Argument of Perigee: " << argument_of_perigee << " degrees" << std::endl;
    std::cout << "Mean Anomaly: " << mean_anomaly << " degrees" << std::endl;
    std::cout << "Mean Motion: " << mean_motion << " revs per day" << std::endl;
    std::cout << "Checksum (Line 1): " << checksum_line1 << ", Checksum (Line 2): " << checksum_line2 << std::endl;
}

std::string Satellite::getFormattedTLE() const {
    std::stringstream ss;
    ss << std::setprecision(4) << std::fixed;
    ss << name << "\n";
    ss << "1 " << std::setw(5) << satellite_number << classification << " "
        << launch_year << launch_number << launch_piece << " "
        << std::setw(2) << std::setfill('0') << static_cast<int>(epoch_year) % 100 // Extract last two digits
        << std::setw(12) << std::setfill(' ') << epoch_day << " "
        << mean_motion_derivative << " "
        << mean_motion_second_derivative << " "
        << bstar_drag << " "
        << ephemeris_type << " "
        << element_set_number << " "
        << checksum_line1 << "\n";
    ss << "2 " << std::setw(5) << satellite_number << " "
        << inclination << " "
        << raan << " "
        << eccentricity << " "
        << argument_of_perigee << " "
        << mean_anomaly << " "
        << mean_motion << " "
        << revolution_number_at_epoch << " "
        << checksum_line2;
    return ss.str();
}

*/