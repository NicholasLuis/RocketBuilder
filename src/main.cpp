# include <../src/global.h>

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

    while (guiManager.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop();
    ConsoleManager::getInstance().stop();
    return 0;
}