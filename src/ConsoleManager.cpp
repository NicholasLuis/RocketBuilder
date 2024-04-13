#include "ConsoleManager.h"

ConsoleManager& ConsoleManager::getInstance() {
    static ConsoleManager instance;
    return instance;
}

ConsoleManager::ConsoleManager() : active(true), worker(&ConsoleManager::processQueue, this) {}

ConsoleManager::~ConsoleManager() {
    stop();
}

void ConsoleManager::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    messageQueue.push(message);
    cv.notify_one();
}

void ConsoleManager::print(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    messageQueue.push(message);
    cv.notify_one();
}

void ConsoleManager::run() {
    active = true;
    if (!worker.joinable()) {
        worker = std::thread(&ConsoleManager::processQueue, this);
    }
}

void ConsoleManager::stop() {
    active = false;
    cv.notify_one();
    if (worker.joinable()) {
        worker.join();
    }
}

void ConsoleManager::processQueue() {
    std::unique_lock<std::mutex> lock(mtx);
    while (active || !messageQueue.empty()) {
        if (messageQueue.empty()) {
            cv.wait(lock, [this] { return !active || !messageQueue.empty(); });
        }
        else {
            std::cout << messageQueue.front() << std::endl;
            messageQueue.pop();
        }
    }
}
