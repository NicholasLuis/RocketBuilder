// GuiManager.h
# pragma once

# include <../src/Rocket.h>
# include <../src/booster.h>
# include <../src/orbit.h>
# include <../src/satellite.h>
# include <../src/Rocket.h>
# include <../src/ConsoleManager.h>
# include <GLFW/glfw3.h>
# include <../lib/imgui/imgui.h>
# include <../lib/imgui/imgui_impl_glfw.h>
# include <../lib/imgui/imgui_impl_opengl3.h>
# include <vector>
# include <optional>
# include <filesystem>
# include <thread>

namespace fs = std::filesystem; 			   // Namespace alias for std::filesystem

class TotalRocket;                              // Forward declaration of TotalRocket class

class GuiManager { 							    // GUI Manager class
public:
    GuiManager();                               // Constructor
    ~GuiManager();                              // Destructor

    void start();                               // Start GUI
    void stop();                                // Stop GUI
    void run();                                 // Run GUI
    void RocketBuilder();                       // Rocket Builder GUI
    bool isRunning() const {                    // Check if GUI is running
        return running;
    }

private:
    GLFWwindow* window;                         // Window handle
    std::thread guiThread;                      // GUI thread
    bool running = false;                       // GUI running flag

    // GUI state management using enum with bitwise operators
    enum DialogFlags {
        guiState = 0,                           // No dialogs open
        RocketBuilderDialog = 1 << 0,           // Bit 0: Dialog to build/edit rocket configurations
        RocketPropertiesDialog = 1 << 1,        // Bit 1: Dialog showing rocket properties and controls
        RocketDialog = 1 << 2,				    // Bit 2: Rocket dialog
        LoadFileDialog = 1 << 3,                // Bit 3: File load dialog
        TleDisplayDialog = 1 << 4,              // Bit 4: TLE display dialog
        LoadRocketDialog = 1 << 5,              // Bit 5: Rocket load dialog
        SatFileListDialog = 1 << 6,             // Bit 6: Satellite file list dialog
    };
    // int guiState = 0;

    std::vector<fs::path> tleFiles;                         // List of TLE files in the directory
    std::optional<Satellite> loadedSatellite;               // Satellite loaded from TLE file
   TotalRocket* totalRocket;                                // TotalRocket object to manage rocket and satellite objects

    void initializeGui(); 					                            // Initialize GUI  
    void mainLoop();							                        // Main GUI loop
    void cleanup();								                        // Cleanup GUI
    void displayFiles(const std::vector<fs::path>& files); 	            // Display files in the directory
    std::vector<fs::path> listTLEFiles(const fs::path& directory);      // List TLE files in the directory
    std::string sanitizeFilePath(const std::string& input); 			// Sanitize file path
    void displayGui(); 						                            // Display GUI
};