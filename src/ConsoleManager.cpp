# include "ConsoleManager.h"

// Singleton instance
ConsoleManager& ConsoleManager::getInstance() {     
    static ConsoleManager instance;                                     // Guaranteed to be destroyed.
    return instance;                                                    // Instantiated on first use.
}

// Constructor
ConsoleManager::ConsoleManager() : active(true), worker(&ConsoleManager::processQueue, this) {}

// Destructor
ConsoleManager::~ConsoleManager() {
    stop();
}

// Log a message to the console
void ConsoleManager::log(const std::string& message) { 
    std::lock_guard<std::mutex> lock(mtx);                              // Lock the console
    messageQueue.push(message); 									    // Log the message
    cv.notify_one(); 												    // Notify the logging thread
}

// Start the logging thread
void ConsoleManager::run() {
    active = true;                                                      // Set the active flag
    if (!worker.joinable()) { 									        // If the worker thread is not joinable
        worker = std::thread(&ConsoleManager::processQueue, this);      // Start the worker thread
    }
}

// Signal to terminate the logging thread
void ConsoleManager::stop() {
    active = false; 												    // Set the active flag
    cv.notify_one(); 												    // Notify the logging thread
    if (worker.joinable()) { 										    // If the worker thread is joinable
        worker.join(); 											        // Join the worker thread
    }
}

// Process the message queue
void ConsoleManager::processQueue() {
    std::unique_lock<std::mutex> lock(mtx); 						    // Lock the console
    while (active || !messageQueue.empty()) { 						    // While the logging thread is active or the message queue is not empty
        if (messageQueue.empty()) { 								    // If the message queue is empty
            cv.wait(lock, [this] { return !active || !messageQueue.empty(); }); // Wait for a new message or termination
        } 
        else { 													        // If the message queue is not empty
            std::cout << messageQueue.front() << std::endl; 		    // Log the message
            messageQueue.pop(); 									    // Remove the message from the queue
        }
    }
}
