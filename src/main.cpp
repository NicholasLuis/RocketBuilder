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

    // Console interaction ----------------
        TotalRocket totalRocket;
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
        std::vector<RocketStage*> rocketStages(numStages);
        
        for (int i = 0; i < numStages; i++)
        {
            double inputStruct, inputFuel, inputImp;

            std::cout << "Enter the structural (empty) mass of stage " << std::to_string((i + 1)) << std::endl;
            std::cin >> inputStruct;

            std::cout << "Enter the fuel mass of stage " << std::to_string((i + 1)) << std::endl;
            std::cin >> inputFuel;

            std::cout << "Enter the engines' specific impulse of stage " << std::to_string((i + 1)) << std::endl;
            std::cin >> inputImp;

            rocketStages[i] = new RocketStage(inputStruct, inputFuel, inputImp);
        }


    // End of console interaction ----------------

    while (guiManager.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop();
    ConsoleManager::getInstance().stop();

    // example rocket for Falcon 9
  /* std::vector<RocketStage*>rocket_stages;

  //  rocket_stages.push_back(new RocketStage(25600, 395700, 283)); //stage 1 of the rocket 
    //rocket_stages.push_back(new RocketStage(3900, 92620, 348)); // satge 2 of the rocket 

    auto stage1 = new RocketStage(25600, 395700, 283);


    TotalRocket totalRocket();


    totalRocket.addToRocket(stage1);

    // Add each RocketStage object to TotalRocket
  /*  for (auto stage : rocket_stages) {
        totalRocket.addToRocket(stage1);
    }

    // Calculate total deltaV of the rocket
    double totalDeltaV = totalRocket.getDeltaV();
    std::cout << "Total DeltaV of the rocket: " << totalDeltaV << " m/s" << std::endl;

    // Clean up memory by deleting the RocketStage objects
    for (auto stage : rocket_stages) {
        delete stage;
    }

    */

    return 0;
}