// satellite.cpp
# include <../src/satellite.h>


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