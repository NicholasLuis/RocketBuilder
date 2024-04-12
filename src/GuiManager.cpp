// GuiManager.cpp
# include <../src/GuiManager.h>


GuiManager::GuiManager() : window(nullptr) {
    // Constructor
}

GuiManager::~GuiManager() {
    // Destructor
    stop();
}

void GuiManager::start() {
    running = true;
    guiThread = std::thread(&GuiManager::run, this);
}

void GuiManager::stop() {
    if (running) {
        running = false;
        if (guiThread.joinable()) {
            guiThread.join();
        }
    }
}

void GuiManager::run() {
    initializeGui();
    mainLoop();
    cleanup();
}

void GuiManager::initializeGui() {
    // Initialize GLFW
    if (!glfwInit()) return;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(2560, 1440, "SAT Launcher", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;
}

void GuiManager::mainLoop() {
    // GUI loop
    while (running && !glfwWindowShouldClose(window)) {
        displayGui();
    }
}

void GuiManager::cleanup() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GuiManager::displayFiles(const std::vector<fs::path>& files) {
    for (const auto& file : files) {
        std::cout << file.filename() << std::endl;
    }
}

std::vector<fs::path> GuiManager::listTLEFiles(const fs::path& directory) {
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".tle") {
            files.push_back(entry.path());
        }
    }
    return files;
}

std::vector<fs::path> GuiManager::listTXTFiles(const fs::path& directory) {
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry.path());
        }
    }
    return files;
}

std::string GuiManager::sanitizeFilePath(const std::string& input) {
    std::string path = input;
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    path.erase(std::find_if(path.rbegin(), path.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), path.end());
    return path;
}

void GuiManager::displayGui() {
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

  


    // ____________________________________________________________
    // TLE PROGRAM

      // Main window code
    ImGui::Begin("TLE Loader");

    if (ImGui::Button("Load TLE from file")) {
        guiState |= LoadFileDialog; // Set the Load File dialog bit
    }

    // Load file dialog
    if (guiState & LoadFileDialog) { // Check if the Load File dialog bit is set
        ImGui::OpenPopup("Load TLE File");
        static char buf[1024] = "";
        if (ImGui::BeginPopupModal("Load TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Load TLE File", ImVec2(700, 400), ImGuiCond_FirstUseEver); // Adjust size as needed
            ImGui::InputText("File Path", buf, IM_ARRAYSIZE(buf));
            if (ImGui::Button("Load")) {
                try {
                    std::string sanitizedPath = sanitizeFilePath(buf);
                    loadedSatellite = Satellite(sanitizedPath); // Directly instantiate
                    guiState |= TleDisplayDialog; // Set the TLE Display dialog bit
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    ImGui::OpenPopup("Error Loading File");
                }
                guiState &= ~LoadFileDialog; // Clear the Load File dialog bit
                memset(buf, 0, sizeof(buf)); // Clear the buffer after loading
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                guiState &= ~LoadFileDialog; // Clear the Load File dialog bit
                memset(buf, 0, sizeof(buf)); // Clear the buffer on cancel
            }
            ImGui::EndPopup();
        }
    }

    if (ImGui::Button("Select from available TLE data")) {
        tleFiles = listTLEFiles("./data"); // Assuming the TLE files are stored in "./data"
        guiState |= SatFileListDialog; // Set the Sat File List dialog bit
    }

    // Satellite file list dialog
    if (guiState & SatFileListDialog) {
        ImGui::OpenPopup("Select TLE File");
        if (ImGui::BeginPopupModal("Select TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Select TLE File", ImVec2(700, 400), ImGuiCond_FirstUseEver); // Adjust size as needed
            for (auto& file : tleFiles) {
                if (ImGui::Button(file.filename().string().c_str())) {
                    try {
                        loadedSatellite.emplace(file.string()); // Correct instantiation using emplace
                        guiState |= TleDisplayDialog; // Set the TLE Display dialog bit
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error loading TLE: " << e.what() << std::endl;
                        ImGui::OpenPopup("Error Loading File");
                    }
                    guiState &= ~SatFileListDialog; // Clear the Sat File List dialog bit
                }
            }
            if (ImGui::Button("Cancel")) {
                guiState &= ~SatFileListDialog; // Clear the Sat File List dialog bit
            }
            ImGui::EndPopup();
        }
    }


    // Display loaded satellite details in a popup
    if (guiState & TleDisplayDialog && loadedSatellite) {
        ImGui::OpenPopup("Satellite TLE");
        if (ImGui::BeginPopupModal("Satellite TLE", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Satellite TLE", ImVec2(700, 400), ImGuiCond_FirstUseEver); // Adjust size as needed
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
                guiState &= ~TleDisplayDialog; // Clear the TLE Display dialog bit
                // Optionally reset loadedSatellite or other states as needed
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();


    // ______________________________________________________________
    // ROCKET BUILDING PROGRAM

    ImGui::Begin("Rocket Builder");

    if (ImGui::Button("Build from scratch")) {
        guiState |= BuildRocketDialog; // Set the Build Rocket dialog bit
    }

    if (guiState & LoadRocketDialog) {
        // LoadRocket logic
        guiState &= ~LoadRocketDialog; // Clear the bit after use
    }

    if (guiState & BuildRocketDialog) {
        RocketBuilder(); // Call the RocketBuilder function
    } 
    if (ImGui::Button("Select from the pre-made rockets")) {
        txtFiles = listTXTFiles("./premadeRockets"); // Assuming the txt files are stored in "./premadeRockets"
        guiState |= SatFileListDialog; // Set the Sat File List dialog bit
    }
    // Satellite file list dialog
    if (guiState & LoadRocketDialog) {
        ImGui::OpenPopup("Select .txt File");
        if (ImGui::BeginPopupModal("Select .txt File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Select .txt File", ImVec2(700, 400), ImGuiCond_FirstUseEver); // Adjust size as needed
            for (auto& file : txtFiles) {
                if (ImGui::Button(file.filename().string().c_str())) {
                    try {
                        loadedSatellite.emplace(file.string()); // Correct instantiation using emplace
                        guiState |= LoadRocketDialog; // Set the TLE Display dialog bit
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error loading txt: " << e.what() << std::endl;
                        ImGui::OpenPopup("Error Loading File");
                    }
                    guiState &= ~LoadRocketDialog; // Clear the Sat File List dialog bit
                }
            }
            if (ImGui::Button("Cancel")) {
                guiState &= ~LoadRocketDialog; // Clear the Sat File List dialog bit
            }
            ImGui::EndPopup();
        }
    }


    ImGui::End();


    //______________________________________________________________
    // ENDING
    
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

void GuiManager::RocketBuilder() {
    static int numStages = 1;
    ImGui::InputInt("Number of Stages", &numStages);
    numStages = std::max(1, numStages); // Ensure at least one stage

    static std::vector<RocketStage> stages(numStages);

    if (ImGui::BeginTabBar("Stages Tab Bar")) {
        for (int i = 0; i < numStages; ++i) {
            std::string tabName = "Stage " + std::to_string(i + 1);
            if (ImGui::BeginTabItem(tabName.c_str())) {
                static double structMass = stages[i].getStructureMass();
                static double fuelMass = stages[i].getFuelMass();
                static double isp = stages[i].getI_sp();

                if (ImGui::InputDouble("Structural Mass (kg)", &structMass)) {
                    stages[i].setStructureMass(structMass);
                }
                if (ImGui::InputDouble("Fuel Mass (kg)", &fuelMass)) {
                    stages[i].setFuelMass(fuelMass);
                }
                if (ImGui::InputDouble("Specific Impulse (s)", &isp)) {
                    stages[i].setI_sp(isp);
                }
                ImGui::EndTabItem();
            }
        }
        if (ImGui::BeginTabItem("Payload Stage")) {
            // Define payload stage inputs (e.g., orbital characteristics)
            static Orbit payloadOrbit;
            ImGui::InputDouble("Desired Orbit Altitude (km)", &payloadOrbit.altitude);
            ImGui::InputDouble("Desired Orbit Inclination (degrees)", &payloadOrbit.inclination);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
        guiState &= ~BuildRocketDialog; // Clear the bit after use
    }
}


