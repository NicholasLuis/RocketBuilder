# include <../src/global.h>

// Log Function to Control Log Messages
void log(const std::string& str) {
    ConsoleManager::getInstance().log(str);
}
template<typename T>
void log(const std::string& str, T& var) {
    ConsoleManager::getInstance().log(str, var);
}


int main() {
    GuiManager guiManager;                          // Create GuiManager object
    ConsoleManager::getInstance().run();			// Run ConsoleManager
    guiManager.start();							    // Start GUI
  
    // create threads to check console log
    std::thread t1([] { log("Checking Console Log... [1]"); });
    std::thread t2([] { log("Checking Console Log... [2]"); });
    std::thread t3([] { log("Checking Console Log... [3]"); });
    std::thread t4([] { log("Checking Console Log... [4]"); });
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    /* To test ConsoleManager
    int stages;
    double length;
    std::string name;
    char option;

    log("Enter integer:", stages);
    log("Enter double:", length);
    log("Enter string:", name);
    log("Enter char:", option);
    */

    std::chrono::milliseconds timespan(1000);

    // Console interaction ------------------------------------------------
    // initializing variables & objects
    TotalRocket totalRocket;
    Orbit orbit;
    uint8_t numStages;
    int input;

    log("Enter the number of stages",input); 

    while (input <= 0)
    {
        input = NULL;
        log("That is an invalid input. Please enter the number of stages again.",input);
    }

    numStages = input;

    for (int i = 0; i < numStages; i++)
    {
        double inputStruct = 0, inputFuel = 0, inputImp = 0;
        std::ostringstream msg;
        msg << "Enter the structure mass of stage " << (i + 1) << ": ";
        log(msg.str(), inputStruct);
        msg.str("");
        msg.clear();

        msg << "Enter the fuel mass of stage " << (i + 1) << ": ";
        log(msg.str(), inputFuel);
        msg.str("");
        msg.clear();

        msg << "Enter the engines' specific impulse of stage " << (i + 1);
        log(msg.str(), inputImp);
        msg.str("");
        msg.clear();

        totalRocket.addToRocket(new RocketStage(inputStruct, inputFuel, inputImp));
    }

    log("Where is the rocket?\n\t0 if it's in space already\n\t1 if it's on Earth", input);
    if (input == 1) // Possiblities if it were to launch from earth
    {
        log("Enter the lattitude of the launch site in degrees", input);
        orbit.setCoords(input, NULL);

        std::ostringstream msg;
        msg << "Enter the amount of fuel you would like to use. (The rocket curruently has " << totalRocket.getFuelMass() << " kg of fuel)";
        log(msg.str(), input);
        msg.str("");
        msg.clear();

        while ((input > totalRocket.getFuelMass()) || (input < 0)) // Ensures the user enters a valid number
        {
            msg << "That is not a valid input. Make sure it is a number between 0 and " << totalRocket.getFuelMass();
            log(msg.str(), input);
            msg.str("");
            msg.clear();
        }

        msg << "There is " << totalRocket.getDeltaV() << "km/s of delta V available";
        log(msg.str());
        msg.str("");
        msg.clear();
        orbit.launchPossibilities(totalRocket.getDeltaV(input)); // prints out the launch possibilities 
        orbit.inclinationPossibilities(); // prints out the possible orbit inclinations attainable from 
    }
    else // it's in space already
    {
        log("What is the current altitude of the rocket (satellite)?", input);
        while (input < 0) // Ensures the user enters a valid number
        {
            log("That is not a valid input. Make sure it is a number greater than 0", input);
        }
        orbit.setRadius(input);

        std::ostringstream msg;
        msg << "Enter the amount of fuel you would like to use. (The rocket curruently has "
            << std::to_string(totalRocket.getFuelMass()) << " kg of fuel)";
        log(msg.str(), input);
        msg.str("");
        msg.clear();
        while ((input > totalRocket.getFuelMass()) || (input < 0)) // Ensures the user enters a valid number
        {
            msg << "That is not a valid input. Make sure it is a number between 0 and "
                << totalRocket.getFuelMass();
            log(msg.str(), input);
            msg.str("");
            msg.clear();
        }
        msg << "There is " << std::to_string(totalRocket.getDeltaV()) << "km/s of delta V available" ;
        log(msg.str());
        msg.str("");
        msg.clear();
        orbit.launchPossibilities(totalRocket.getDeltaV(input)); // prints out the launch possibilities 
        orbit.inclinationPossibilities(totalRocket.getDeltaV(input), orbit.getRadius()); // prints out the possible orbit inclinations attainable from its current orbit
    }

    // End of console interaction ------------------------------------------------

    while (guiManager.isRunning()) {    
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop(); 							    // Stop GUI
    ConsoleManager::getInstance().stop();			// Stop ConsoleManager
    return 0;
}