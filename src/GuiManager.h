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

namespace fs = std::filesystem;

class TotalRocket;

class GuiManager {
public:
    GuiManager();
    ~GuiManager();

    void start();
    void stop();
    void run();
    void RocketBuilder();
    bool isRunning() const {
        return running;
    }

private:
    GLFWwindow* window;
    std::thread guiThread;
    bool running = false;

    // GUI state management using enum with bitwise operators
    enum DialogFlags {
        NoDialog = 0,             // No dialogs open
        RocketBuilderDialog = 1 << 0,  // Dialog to build/edit rocket configurations
        RocketPropertiesDialog = 1 << 1, // Dialog showing rocket properties and controls
        RocketDialog = 1 << 2,
        LoadFileDialog = 1 << 3,    // File load dialog
        TleDisplayDialog = 1 << 4,  // TLE display dialog
        LoadRocketDialog = 1 << 5,   // Rocket load dialog
        SatFileListDialog = 1 << 6, // Bit 6
    };


    int guiState = NoDialog;  // Initial state with no dialogs open

    std::vector<fs::path> tleFiles;
    std::vector<fs::path> txtFiles;
    std::optional<Satellite> loadedSatellite;
   TotalRocket* totalRocket;

    void initializeGui();
    void mainLoop();
    void cleanup();
    void displayFiles(const std::vector<fs::path>& files);
    std::vector<fs::path> listTLEFiles(const fs::path& directory);
    std::vector<fs::path> listTXTFiles(const fs::path& directory);
    std::string sanitizeFilePath(const std::string& input);
    void displayGui();
};