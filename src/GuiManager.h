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
    void RocketBuilder();
    bool isRunning() const {
        return running;
    }

private:
    GLFWwindow* window;
    std::thread guiThread;
    bool running = false;

    // GUI state
    enum DialogFlags {
        LoadFileDialog = 1 << 0,    // Bit 0
        SatFileListDialog = 1 << 1, // Bit 1
        TleDisplayDialog = 1 << 2,  // Bit 2
        LoadRocketDialog = 1 << 3,  // Bit 3
        BuildRocketDialog = 1 << 4  // Bit 4
    };
    int guiState = 0;

    std::vector<fs::path> tleFiles;
    std::vector<fs::path> txtFiles;
    std::optional<Satellite> loadedSatellite;

    void initializeGui();
    void mainLoop();
    void cleanup();
    void displayFiles(const std::vector<fs::path>& files);
    std::vector<fs::path> listTLEFiles(const fs::path& directory);
    std::vector<fs::path> listTXTFiles(const fs::path& directory);
    std::string sanitizeFilePath(const std::string& input);
    void displayGui();
};