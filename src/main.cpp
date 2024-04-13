# include <../src/global.h>

// Log Function to Control Log Messages
void log(const std::string& str) {
    ConsoleManager::getInstance().log(str);
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

    while (guiManager.isRunning()) {    
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop(); 							    // Stop GUI
    ConsoleManager::getInstance().stop();			// Stop ConsoleManager
    return 0;
}