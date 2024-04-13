// satellite.cpp
# include <../src/satellite.h>

// Template for generic type (string assignment)
template<typename T>
void assign_from_match(T& variable, const std::smatch& matches, size_t index) {
    if constexpr (std::is_same_v<T, int>) {
        // Convert string to int
        variable = std::stoi(matches[index].str());
    }
    else if constexpr (std::is_same_v<T, double>) {
        // Convert string to double
        variable = std::stod(matches[index].str());
    }
    else if constexpr (std::is_same_v<T, char>) {
        // Assign only the first character of the string
        variable = matches[index].str()[0];
    }
    else {
        // Direct assignment for strings or other compatible types
        variable = matches[index].str();  // Make sure T is compatible with std::string assignment
    }
}

// Overload for double type with optional processing
void assign_from_match_double(double& variable, const std::smatch& matches, size_t index, int exponent, bool decimal) {
    if (decimal) {
        std::string temp = "0." + matches[index].str();
        variable = std::stod(temp) * pow(10, exponent);
    }
    else {
        variable = std::stod(matches[index].str());
    }
}
// Function to convert string by prefixing "0." and converting to double
template<typename T>
void assign_from_prefixed_decimal(T& variable, const std::smatch& matches, size_t index) {
    std::string temp = "0." + matches[index].str();
    variable = std::stod(temp);
}

// Function to convert string by prefixing "0.", converting to double and scaling by a power of 10
template<typename T>
void assign_from_scaled_decimal(T& variable, const std::smatch& matches, size_t index, int exponent_index) {
    std::string temp = "0." + matches[index].str();
    int exponent = -std::stoi(matches[exponent_index].str());
    variable = std::stod(temp) * pow(10, exponent);
}

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
        assign_from_match(satellite_number, matches, 1);
        assign_from_match(classification, matches, 2);
        assign_from_match(launch_year, matches, 3);
        assign_from_match(launch_number, matches, 4);
        assign_from_match(launch_piece, matches, 5);
        assign_from_match(epoch_year, matches, 6);
        epoch_year += 2000; // Adjust for full year
        assign_from_match(epoch_day, matches, 7);
        assign_from_match(mean_motion_derivative, matches, 8);
        assign_from_scaled_decimal(mean_motion_second_derivative, matches, 9, 10);
        assign_from_scaled_decimal(bstar_drag, matches, 11, 12);
        assign_from_match(ephemeris_type, matches, 13);
        assign_from_match(element_set_number, matches, 14);
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
        // Skipping the first match since it's not used here
        assign_from_match(inclination, matches, 2);
        assign_from_match(raan, matches, 3);
        assign_from_match(eccentricity, matches, 4); // No Instance of Overloaded Function "assign_from_match" matches the argument list
        assign_from_match(argument_of_perigee, matches, 5);
        assign_from_match(mean_anomaly, matches, 6);
        assign_from_match(mean_motion, matches, 7);
        assign_from_match(revolution_number_at_epoch, matches, 8);
    }
    else {
        throw std::runtime_error("Failed to parse line 2 of TLE");
    }
}