#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>

#ifndef CONSOLE_MANAGER_H
#define CONSOLE_MANAGER_H
class ConsoleManager {
public:
    static ConsoleManager& getInstance();

    void log(const std::string& message);
    void print(const std::string& message);
    void run();  // Start the logging thread
    void stop(); // Signal to terminate the logging thread

private:
    ConsoleManager();
    ~ConsoleManager();
    ConsoleManager(const ConsoleManager&) = delete;
    ConsoleManager& operator=(const ConsoleManager&) = delete;

    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::string> messageQueue;
    bool active;
    std::thread worker;
    void processQueue();
};

#endif