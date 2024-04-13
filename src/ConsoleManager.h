# include <iostream>
# include <mutex>
# include <queue>
# include <string>
# include <thread>
# include <condition_variable>

# ifndef CONSOLE_MANAGER_H
# define CONSOLE_MANAGER_H

class ConsoleManager {
public:
    static ConsoleManager& getInstance();                           // Singleton instance

    void log(const std::string& message); // For logging messages without input
    template<typename T>
    void log(const std::string& message, T& var);
    void run();                                                     // Start the logging thread
    void stop();                                                    // Signal to terminate the logging thread

private:
    ConsoleManager(); 					                            // Private constructor
    ~ConsoleManager(); 				                                // Destructor
    ConsoleManager(const ConsoleManager&) = delete;                 // Copy constructor
    ConsoleManager& operator=(const ConsoleManager&) = delete; 	    // Copy assignment operator

    std::mutex mtx; 											    // Mutex to protect the message queue
    std::condition_variable cv; 								    // Condition variable to signal new messages
    std::queue<std::string> messageQueue; 						    // Queue of messages to log
    bool active; 												    // Flag to indicate if the logging thread is active
    std::thread worker; 										    // Thread to process the message queue
    void processQueue(); 										    // Process the message queue
};

# endif