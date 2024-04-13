// GuiManager.cpp
# include <../src/GuiManager.h>


GuiManager::GuiManager() : window(nullptr), totalRocket(new TotalRocket) {
    // Constructor
}

GuiManager::~GuiManager() {
    // Destructor
    stop();
    delete totalRocket;
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
    if (ImGui::Begin("Rocket Builder")) {
        if (ImGui::Button("Load Rocket")) {
            guiState |= LoadRocketDialog; // Set the Load Rocket dialog bit
        }
        if (ImGui::Button("Build from scratch")) {
            guiState |= RocketBuilderDialog; // Set the Build Rocket dialog bit
        }
        ImGui::End(); // Close the main "Rocket Builder" window
    }

    // Dialog for building or editing the rocket
    if (guiState & RocketBuilderDialog) {
        ImGui::Begin("Rocket Builder", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        RocketBuilder(); // Function to handle the building of the rocket
        if (ImGui::Button("Done")) {
            guiState |= RocketPropertiesDialog; // Open rocket properties dialog
            guiState &= ~RocketBuilderDialog;  // Close builder dialog
        }
        ImGui::End();
    }

    // Dialog for loading a rocket
    if (guiState & LoadRocketDialog) {
        ImGui::Begin("Load Rocket", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        // UI elements for loading a rocket could go here
        if (ImGui::Button("Load")) {
            // Implement the actual loading logic here
            guiState &= ~LoadRocketDialog; // Assume load is successful, close this dialog
        }
        if (ImGui::Button("Cancel")) {
            guiState &= ~LoadRocketDialog; // Clear the bit after use
        }
        ImGui::End();
    }

    // Dialog for displaying and managing rocket properties
    if (guiState & RocketPropertiesDialog) {
        if (ImGui::Begin("Built Rocket", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Built Rocket", ImVec2(700, 400), ImGuiCond_FirstUseEver); // Adjust size as needed
            ImGui::Text("Rocket Name: %s", totalRocket->getName().c_str());
            ImGui::Text("Total Structure Mass: %d", totalRocket->getStructureMass());
            ImGui::Text("Total Fuel Mass: %d", totalRocket->getFuelMass());
            ImGui::Text("DeltaV: %d", totalRocket->getDeltaV());


            // Dispalys
            if (ImGui::Button("Edit")) {
                guiState |= RocketBuilderDialog;  // Reopen rocket builder dialog
                guiState &= ~RocketPropertiesDialog; // Close properties dialog temporarily
            }
            if (ImGui::Button("Destroy")) {
                while (!totalRocket->getStageQueue().empty()) {
                    totalRocket->detachStage(); // Detach and deconstruct all stages
                }
                guiState &= ~RocketPropertiesDialog; // Close properties dialog permanently
            }
            ImGui::End();
        }
    }


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
    static char name[1024] = "";
    ImGui::InputText("Rocket Name", name, IM_ARRAYSIZE(name)); // Input for the rocket name
    totalRocket->setName(name); // Set the rocket name
    ImGui::InputInt("Number of Stages", &numStages);
    numStages = std::max(1, numStages); // Ensure at least one stage

    // Ensure the rocket has the correct number of stages
    while (totalRocket->getStageQueue().size() < numStages) {
        totalRocket->addToRocket(new RocketStage(0,0,0,0)); // Add stages as needed
    }
    while (totalRocket->getStageQueue().size() > numStages) {
        totalRocket->detachStage(); // Remove stages in excess
    }

    if (ImGui::BeginTabBar("Stages Tab Bar")) {
        // Make a copy of the original queue for safe iteration
        auto tempQueue = totalRocket->getStageQueue(); // Copy the queue
        std::vector<RocketStage*> tempStages; // Temporary storage to preserve stages
        double structMass = 0.0;
        double fuelMass = 0.0;
        double isp = 0.0;

        while (!tempQueue.empty()) { // Copy the queue to the temporary vector
			RocketStage* stage = tempQueue.front().get(); 
			tempStages.push_back(stage); 
			tempQueue.pop(); 
		}

        for (int i = 0; i < tempStages.size(); ++i) {
            std::string tabName = "Stage " + std::to_string(i + 1); // Tab name
            if (ImGui::BeginTabItem(tabName.c_str())) { // Begin tab item

                // Get the mass values
                double structMass = tempStages[i]->getStructureMass();  
                double fuelMass = tempStages[i]->getFuelMass(); 
                double isp = tempStages[i]->getI_sp(); 

                ImGui::Text("Structural Mass (kg):");
                if (ImGui::InputDouble("##StructuralMass", &structMass)) {
                    tempStages[i]->setStructureMass(structMass); // Update the stage Structure mass
                }

                ImGui::Text("Fuel Mass (kg):");
                if (ImGui::InputDouble("##FuelMass", &fuelMass)) {
                    tempStages[i]->setFuelMass(fuelMass); // Update the stage Fuel mass
                }

                ImGui::Text("Specific Impulse (s):");
                if (ImGui::InputDouble("##SpecificImpulse", &isp)) {
                    tempStages[i]->setI_sp(isp); // Update the stage Isp
                }

                ImGui::EndTabItem();
            }
        }

        // Re-populate the original queue with the updated stages
        //while (!totalRocket->getStageQueue().empty()) {
        //    totalRocket->detachStage(); // Empty the original queue
        //}
        //for (RocketStage* stage : tempStages) {
        //    totalRocket->addToRocket(stage); // Re-add the updated stages
        //}

        ImGui::EndTabBar(); // End the tab bar
    }

    if (ImGui::Button("Cancel")) {
        // Clear the rocket and reset the dialog bit accordingly
        while (!totalRocket->getStageQueue().empty()) { 
            totalRocket->detachStage(); 
        }
        guiState &= ~RocketBuilderDialog; // Clear the Rocket Builder dialog bit
    }
}