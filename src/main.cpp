# include <../src/booster.h>
# include <../src/orbit.h>
# include <../src/satellite.h>

# include <GLFW/glfw3.h>
# include <../lib/imgui/imgui.h>
# include <../lib/imgui/imgui_impl_glfw.h>
# include <../lib/imgui/imgui_impl_opengl3.h>

# include <iostream>
# include <string>
# include <filesystem>
# include <optional>
# include <regex>

namespace fs = std::filesystem;

void displayFiles(const std::vector<fs::path>& files) {
    for (const auto& file : files) {
        std::cout << file.filename() << std::endl;
    }
}

std::vector<fs::path> listTLEFiles(const fs::path& directory) {
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".tle") {
            files.push_back(entry.path());
        }
    }
    return files;
}

std::string sanitizeFilePath(const std::string& input) {
    std::string path = input;
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    path.erase(std::find_if(path.rbegin(), path.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), path.end());
    return path;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(2560, 1480, "TLE Loader", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Before the main loop
    bool show_load_file_dialog = false;
    bool show_sat_file_list = false;
    bool show_tle_display = false; // Flag to show TLE data in a popup
    std::vector<fs::path> tleFiles; // List of TLE files found
    std::optional<Satellite> loadedSatellite; // Holds the loaded satellite data

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main window code
        ImGui::Begin("TLE Loader");




        // ____________________________________________________________
        // GUI PROGRAM STARTS HERE

        if (ImGui::Button("Load TLE from file")) {
            show_load_file_dialog = true;
        }

        // Load file dialog
        if (show_load_file_dialog) {
            ImGui::OpenPopup("Load TLE File");
            static char buf[1024] = "";
            if (ImGui::BeginPopupModal("Load TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputText("File Path", buf, IM_ARRAYSIZE(buf));
                if (ImGui::Button("Load")) {
                    try {
                        std::string sanitizedPath = sanitizeFilePath(buf);
                        loadedSatellite = Satellite(sanitizedPath); // Directly instantiate
                        show_load_file_dialog = false;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                        ImGui::OpenPopup("Error Loading File");
                    }
                    memset(buf, 0, sizeof(buf)); // Clear the buffer after loading
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    show_load_file_dialog = false;
                    memset(buf, 0, sizeof(buf)); // Clear the buffer on cancel
                }
                ImGui::EndPopup();
            }
        }

        if (ImGui::Button("Select from available TLE data")) {
            tleFiles = listTLEFiles("./data"); // Assuming the TLE files are stored in "./data"
            show_sat_file_list = true;
        }

        // Load file dialog
        if (show_load_file_dialog) {
            ImGui::OpenPopup("Load TLE File");
            static char buf[1024] = "";
            if (ImGui::BeginPopupModal("Load TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::InputText("File Path", buf, IM_ARRAYSIZE(buf));
                if (ImGui::Button("Load")) {
                    try {
                        std::string sanitizedPath = sanitizeFilePath(buf);
                        loadedSatellite.emplace(sanitizedPath); // Use emplace for direct instantiation
                        show_tle_display = true;
                        show_load_file_dialog = false;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                        ImGui::OpenPopup("Error Loading File");
                    }
                    memset(buf, 0, sizeof(buf)); // Clear the buffer after loading
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    show_load_file_dialog = false;
                    memset(buf, 0, sizeof(buf)); // Clear the buffer on cancel
                }
                ImGui::EndPopup();
            }
        }

        // Satellite file list dialog
        if (show_sat_file_list) {
            ImGui::OpenPopup("Select TLE File");
            if (ImGui::BeginPopupModal("Select TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                for (auto& file : tleFiles) {
                    if (ImGui::Button(file.filename().string().c_str())) {
                        try {
                            loadedSatellite.emplace(file.string()); // Correct instantiation using emplace
                            show_tle_display = true;
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Error loading TLE: " << e.what() << std::endl;
                            ImGui::OpenPopup("Error Loading File");
                        }
                        show_sat_file_list = false;
                    }
                }
                if (ImGui::Button("Cancel")) {
                    show_sat_file_list = false;
                }
                ImGui::EndPopup();
            }
        }
      

        // Display loaded satellite details in a popup
        if (show_tle_display && loadedSatellite) {
            ImGui::OpenPopup("Satellite TLE");
            if (ImGui::BeginPopupModal("Satellite TLE", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Satellite Name: %s", loadedSatellite->getName().c_str());
                ImGui::Text("Satellite Number: %d", loadedSatellite->getSatelliteNumber());
                ImGui::Text("Classification: %c", loadedSatellite->getClassification());
                ImGui::Text("Launch Year: %s", loadedSatellite->getLaunchYear().c_str());
                ImGui::Text("Launch Number: %s", loadedSatellite->getLaunchNumber().c_str());
                ImGui::Text("Piece of the Launch: %s", loadedSatellite->getLaunchPiece().c_str());
                ImGui::Text("Epoch Year: %.1f", loadedSatellite->getEpochYear());
                ImGui::Text("Epoch Day: %.1f", loadedSatellite->getEpochDay());
                ImGui::Text("Inclination: %.4f", loadedSatellite->getInclination());
                ImGui::Text("RAAN: %.4f", loadedSatellite->getRAAN());
                ImGui::Text("Eccentricity: %.7f", loadedSatellite->getEccentricity());
                ImGui::Text("Argument of Perigee: %.4f", loadedSatellite->getArgumentOfPerigee());
                ImGui::Text("Mean Anomaly: %.4f", loadedSatellite->getMeanAnomaly());
                ImGui::Text("Mean Motion: %.8f", loadedSatellite->getMeanMotion());
                ImGui::Text("Revolution Number at Epoch: %d", loadedSatellite->getRevolutionNumberAtEpoch());

                if (ImGui::Button("Close")) {
                    show_tle_display = false; // Close the popup
                    // Optionally reset loadedSatellite or other states as needed
                }
                ImGui::EndPopup();
            }
        }

        // ______________________________________________________________
        // GUI PROGRAM ENDSS HERE





        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}