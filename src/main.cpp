# include <../src/global.h>

int main() {
    GuiManager guiManager;
    guiManager.start();

    while (guiManager.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    guiManager.stop();
    return 0;
}