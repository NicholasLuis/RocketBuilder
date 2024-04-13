# include <../src/global.h>
#include <vector>

void log(const std::string& str) {
    ConsoleManager::getInstance().log(str);
}

int main() {
    GuiManager guiManager;
    ConsoleManager::getInstance().run();
    guiManager.start();
  
    std::thread t1([] { log("Hello from T1"); });
    std::thread t2([] { log("Hello from T2"); });
    std::thread t3([] { log("Hello from T3"); });
    std::thread t4([] { log("Hello from T4"); }); 
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // Console interaction ------------------------------------------------
        // initializing variables & objects
        TotalRocket totalRocket;
        Orbit orbit;
        uint8_t numStages;
        int input;

        std::cout << "Enter the number of stages" << std::endl; // Use the object that prints to console using a mutex
        std::cin >> input;

        while (input <= 0)
        {
            input = NULL;
            std::cout << "That is an invalid input. Please enter the number of stages again." << std::endl;
            std::cin >> input;
        }
        
        numStages = input;
        
        for (int i = 0; i < numStages; i++)
        {
            double inputStruct = 0, inputFuel = 0, inputImp = 0;

            std::cout << "Enter the structural (empty) mass of stage " << std::to_string(i + 1) << std::endl;
            std::cin >> inputStruct;

            std::cout << "Enter the fuel mass of stage " << std::to_string((i + 1)) << std::endl;
            std::cin >> inputFuel;

            std::cout << "Enter the engines' specific impulse of stage " << std::to_string((i + 1)) << std::endl;
            std::cin >> inputImp;

            totalRocket.addToRocket(new RocketStage(inputStruct, inputFuel, inputImp));
        }

        std::cout << "Where is the rocket?\n\t0 if it's in space already\n\t1 if it's on Earth" << std::endl;
        std::cin >> input;
        if (input == 1) // Possiblities if it were to launch from earth
        {
            std::cout << "Enter the lattitude of the launch site in degrees" << std::endl;
            std::cin >> input;
            orbit.setCoords(input, NULL);

            std::cout << "Enter the amount of fuel you would like to use. (The rocket curruently has " << totalRocket.getFuelMass() << " kg of fuel)" << std::endl;
            std::cin >> input;
            
            while ((input > totalRocket.getFuelMass()) || (input < 0)) // Ensures the user enters a valid number
            {
                std::cout << "That is not a valid input. Make sure it is a number between 0 and " << std::to_string(totalRocket.getFuelMass()) << std::endl;
                std::cin >> input;
            }

            std::cout << "There is " << std::to_string(totalRocket.getDeltaV()) << "km/s of delta V available" << std::endl;;
            orbit.launchPossibilities(totalRocket.getDeltaV(input)); // prints out the launch possibilities 
            orbit.inclinationPossibilities(); // prints out the possible orbit inclinations attainable from 
        }
        else // it's in space already
        {
            std::cout << "What is the current altitude of the rocket (satellite)?";
            std::cin >> input;
            while (input < 0) // Ensures the user enters a valid number
            {
                std::cout << "That is not a valid input. Make sure it is a number greater than 0" << std::endl;
                std::cin >> input;
            }
            orbit.setRadius(input);

            std::cout << "Enter the amount of fuel you would like to use. (The rocket curruently has " 
                      << std::to_string(totalRocket.getFuelMass()) << " kg of fuel)" << std::endl;
            std::cin >> input;
            while ((input > totalRocket.getFuelMass()) || (input < 0)) // Ensures the user enters a valid number
            {
                std::cout << "That is not a valid input. Make sure it is a number between 0 and "
                    << std::to_string(totalRocket.getFuelMass()) << std::endl;
                std::cin >> input;
            }
            std::cout << "There is " << std::to_string(totalRocket.getDeltaV()) << "km/s of delta V available" << std::endl;;
            orbit.launchPossibilities(totalRocket.getDeltaV(input)); // prints out the launch possibilities 
            orbit.inclinationPossibilities(totalRocket.getDeltaV(input), orbit.getRadius()); // prints out the possible orbit inclinations attainable from its current orbit
        }
        
    // End of console interaction ------------------------------------------------

    while (guiManager.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop();
    ConsoleManager::getInstance().stop();
    return 0;
}