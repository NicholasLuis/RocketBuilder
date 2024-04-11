// GuiManager.h
# pragma once

# include <../src/global.h>

namespace fs = std::filesystem;

class GuiManager {
public:
    GuiManager();
    ~GuiManager();

    void start();
    void stop();
    void run();
    bool isRunning() const {
        return running;
    }

private:
    GLFWwindow* window;
    std::thread guiThread;
    bool running = false;

    // GUI state
    bool show_load_file_dialog = false;
    bool show_sat_file_list = false;
    bool show_tle_display = false;
    std::vector<fs::path> tleFiles;
    std::optional<Satellite> loadedSatellite;

    void initializeGui();
    void mainLoop();
    void cleanup();
    void displayFiles(const std::vector<fs::path>& files);
    std::vector<fs::path> listTLEFiles(const fs::path& directory);
    std::string sanitizeFilePath(const std::string& input);
    void displayGui();
};