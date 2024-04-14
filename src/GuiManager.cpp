// GuiManager.cpp
# include <../src/GuiManager.h>

// Log Function to Control Log Messages
void GuiManager::log(const std::string& str) {
    ConsoleManager::getInstance().log(str);
}
template<typename T>
void GuiManager::log(const std::string& str, T& var) {
    ConsoleManager::getInstance().log(str, var);
}

// Constructor
GuiManager::GuiManager() : window(nullptr), totalRocket(new TotalRocket), orbit(new Orbit) {}

// Destructor
GuiManager::~GuiManager() {
    // Destructor
    stop();
    delete totalRocket;                                 // Delete the totalRocket object
    delete orbit;									    // Delete the orbit object
}

// Start the GUI thread
void GuiManager::start() {
    running = true;                                     // Set the running flag to true
    guiThread = std::thread(&GuiManager::run, this);    // Start the GUI thread
}

// Stop the GUI thread
void GuiManager::stop() {
    if (running) {
        running = false;
        if (guiThread.joinable()) {
            guiThread.join();
        }
    }
}

// Run the GUI
void GuiManager::run() {
    initializeGui();
    mainLoop();
    cleanup();
}

// Initialize the GUI
void GuiManager::initializeGui() {
    // Initialize GLFW
    if (!glfwInit()) return;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(2560, 1440, "SAT Launcher", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);     // Make the window's context current
    glfwSwapInterval(1);                // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();               // Check the version of ImGui
    ImGui::CreateContext();             // Create the ImGui context

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);     // Initialize ImGui for GLFW
    const char* glsl_version = "#version 150";      // Set the GLSL version
    ImGui_ImplOpenGL3_Init(glsl_version);           // Initialize ImGui for OpenGL3
    ImGuiIO& io = ImGui::GetIO();                   // Get the ImGui IO
    io.FontGlobalScale = 2.0f;                      // Set the global font scale
}

// Main GUI loop
void GuiManager::mainLoop() {
    // GUI loop
    while (running && !glfwWindowShouldClose(window)) { 
        displayGui(); 
    }
}

// Cleanup the GUI
void GuiManager::cleanup() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();       // Shutdown ImGui for OpenGL3
    ImGui_ImplGlfw_Shutdown();          // Shutdown ImGui for GLFW
    ImGui::DestroyContext();            // Destroy the ImGui context
    glfwDestroyWindow(window);          // Destroy the window
    glfwTerminate();                    // Terminate GLFW
}

// Display the files in the directory
void GuiManager::displayFiles(const std::vector<fs::path>& files) {
    for (const auto& file : files) {                                    // Iterate over the files
        std::cout << file.filename() << std::endl;                      // Display the file name
    }
}

//  List the TLE files in the directory
std::vector<fs::path> GuiManager::listTLEFiles(const fs::path& directory) {
    std::vector<fs::path> files;                                                    // Vector to store the files
    for (const auto& entry : fs::directory_iterator(directory)) {                   // Iterate over the directory
        if (entry.is_regular_file() && entry.path().extension() == ".tle") {        // Check if the file is a TLE file
            files.push_back(entry.path());                                          // Add the file to the vector
        }
    }
    return files;                                                                   // Return the vector of files
} 

std::vector<fs::path> GuiManager::listFiles(const fs::path& directory) {
    std::vector<fs::path> files;                                                    // Vector to store the files
    for (const auto& entry : fs::directory_iterator(directory)) {                   // Iterate over the directory
        if (entry.is_regular_file()) {       
            files.push_back(entry.path());                                          // Add the file to the vector
        }
    }
    return files;                                                                   // Return the vector of files
}

// Sanitize the file path
std::string GuiManager::sanitizeFilePath(const std::string& input) { 
    std::string path = input;
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), [](unsigned char ch) {  // Erase the leading whitespace
        return !std::isspace(ch);                                                           // Find the first non-whitespace character
        }));
    path.erase(std::find_if(path.rbegin(), path.rend(), [](unsigned char ch) {              // Erase the trailing whitespace
        return !std::isspace(ch);                                                           // Find the first non-whitespace character from the end
        }).base(), path.end());
    return path;                                                                            // Return the sanitized path
}

// Display the GUI
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
        guiState |= LoadFileDialog;                                                                         // Set the Load File dialog bit
    }

    // Load file dialog
    if (guiState & LoadFileDialog) {                                                                        // Check if the Load File dialog bit is set
        ImGui::OpenPopup("Load TLE File");
        static char buf[1024] = "";                                                                         // Buffer for the file path
        if (ImGui::BeginPopupModal("Load TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {             // Begin the popup modal
            ImGui::SetWindowSize("Load TLE File", ImVec2(700, 400), ImGuiCond_FirstUseEver);                // Adjust size as needed
            ImGui::InputText("File Path", buf, IM_ARRAYSIZE(buf));                                          // Input for the file path
            if (ImGui::Button("Load")) {                                                                    // Load button
                try {                                                                                       // Try to load the satellite
                    std::string sanitizedPath = sanitizeFilePath(buf);
                    loadedSatellite = Satellite(sanitizedPath);                                             // Directly instantiate
                    guiState |= TleDisplayDialog;                                                           // Set the TLE Display dialog bit
                }
                catch (const std::exception& e) {                                                           // Catch any exceptions
                    std::cerr << "Error: " << e.what() << std::endl;
                    ImGui::OpenPopup("Error Loading File");                                                 // Open an error popup
                }
                guiState &= ~LoadFileDialog;                                                                // Clear the Load File dialog bit
                memset(buf, 0, sizeof(buf));                                                                // Clear the buffer after loading
            }
            ImGui::SameLine();                                                                              // Display the button on the same line   
            if (ImGui::Button("Cancel")) {                                                                  // Cancel button
                guiState &= ~LoadFileDialog;                                                                // Clear the Load File dialog bit
                memset(buf, 0, sizeof(buf));                                                                // Clear the buffer on cancel
            }
            ImGui::EndPopup();                                                                              // End the popup modal
        }
    }

    if (ImGui::Button("Select from available TLE data")) {                                                  // Button to select from available TLE data
        tleFiles = listTLEFiles("./data");                                                                  // Assuming the TLE files are stored in "./data"
        guiState |= SatFileListDialog;                                                                      // Set the Sat File List dialog bit
    }

    // Satellite file list dialog
    if (guiState & SatFileListDialog) {                                                                     // Check if the Sat File List dialog bit is set
        ImGui::OpenPopup("Select TLE File");                                                                // Open the Select TLE File popup
        if (ImGui::BeginPopupModal("Select TLE File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {           // Begin the popup modal
            ImGui::SetWindowSize("Select TLE File", ImVec2(700, 400), ImGuiCond_FirstUseEver);              // Adjust size as needed
            for (auto& file : tleFiles) {
                if (ImGui::Button(file.filename().string().c_str())) {                                      // Display the file name as a button
                    try {
                        loadedSatellite.emplace(file.string());                                             // Correct instantiation using emplace
                        guiState |= TleDisplayDialog;                                                       // Set the TLE Display dialog bit
                    }
                    catch (const std::exception& e) {                                                       // Catch any exceptions
                        std::cerr << "Error loading TLE: " << e.what() << std::endl;                        // Display error message
                        ImGui::OpenPopup("Error Loading File");                                             // Open an error popup
                    }
                    guiState &= ~SatFileListDialog;                                                         // Clear the Sat File List dialog bit
                }
            }
            if (ImGui::Button("Cancel")) {                                                                  // Cancel button
                guiState &= ~SatFileListDialog;                                                             // Clear the Sat File List dialog bit
            }
            ImGui::EndPopup();                                                                              // End the popup modal
        }
    }


    // Display loaded satellite details in a popup
    if (guiState & TleDisplayDialog && loadedSatellite) {
        ImGui::OpenPopup("Satellite TLE"); // Open the Satellite TLE popup
        if (ImGui::BeginPopupModal("Satellite TLE", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowSize("Satellite TLE", ImVec2(700, 400), ImGuiCond_FirstUseEver);                // Adjust size as needed
            ImGui::Text("Satellite Name: %s", loadedSatellite->getName().c_str());                          // Display the satellite name
            ImGui::Text("Satellite Number: %d", loadedSatellite->getSatelliteNumber());                     // Display the satellite number
            ImGui::Text("Classification: %c", loadedSatellite->getClassification());                        // Display the classification
            ImGui::Text("Launch Year: %s", loadedSatellite->getLaunchYear().c_str());                       // Display the launch year
            ImGui::Text("Launch Number: %s", loadedSatellite->getLaunchNumber().c_str());                   // Display the launch number
            ImGui::Text("Piece of the Launch: %s", loadedSatellite->getLaunchPiece().c_str());              // Display the launch piece
            ImGui::Text("Epoch Year: %.1f", loadedSatellite->getEpochYear());                               // Display the epoch year
            ImGui::Text("Epoch Day: %.1f", loadedSatellite->getEpochDay());                                 // Display the epoch day
            ImGui::Text("Inclination: %.4f", loadedSatellite->getInclination());                            // Display the inclination
            ImGui::Text("RAAN: %.4f", loadedSatellite->getRAAN());                                          // Display the RAAN
            ImGui::Text("Eccentricity: %.7f", loadedSatellite->getEccentricity());                          // Display the eccentricity
            ImGui::Text("Argument of Perigee: %.4f", loadedSatellite->getArgumentOfPerigee());              // Display the argument of perigee
            ImGui::Text("Mean Anomaly: %.4f", loadedSatellite->getMeanAnomaly());                           // Display the mean anomaly
            ImGui::Text("Mean Motion: %.8f", loadedSatellite->getMeanMotion());                             // Display the mean motion
            ImGui::Text("Revolution Number at Epoch: %d", loadedSatellite->getRevolutionNumberAtEpoch());   // Display the revolution number at epoch

            if (ImGui::Button("Close")) { // Close button
                guiState &= ~TleDisplayDialog; // Clear the TLE Display dialog bit
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();


    // ______________________________________________________________
    // ROCKET BUILDING PROGRAM
    if (ImGui::Begin("Rocket Builder")) {           // Begin the main "Rocket Builder" window
        if (ImGui::Button("Load Rocket")) {         // Button to load a rocket
            RocketFiles = listFiles("./premadeRockets");  // List files in the premadeRockets directory
            guiState |= LoadRocketDialog;  // Set the Load Rocket dialog bit
            
        }
        if (ImGui::Button("Build from scratch")) {
            guiState |= RocketBuilderDialog;        // Set the Build Rocket dialog bit
        }
        ImGui::End();                               // Close the main "Rocket Builder" window
    }

    // Dialog for building or editing the rocket
    if (guiState & RocketBuilderDialog) {
        ImGui::Begin("Rocket Builder", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        RocketBuilder();                            // Function to handle the building of the rocket
        if (ImGui::Button("Done")) {
            guiState |= RocketPropertiesDialog;     // Open rocket properties dialog
            guiState &= ~RocketBuilderDialog;       // Close builder dialog
        }
        ImGui::End();
    }


    if (guiState & LoadRocketDialog) {
        ImGui::Begin("Load Rocket", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto& file : RocketFiles) {
            if (ImGui::Button(file.filename().string().c_str())) {
                RocketLoader(file);  // Load the rocket configuration from the file
                guiState |= RocketPropertiesDialog;  // Open rocket properties dialog to display loaded rocket
                guiState &= ~LoadRocketDialog;  // Close the load dialog
                break;  // Exit the loop once a file is clicked
            }
        }
        if (ImGui::Button("Cancel")) {
            guiState &= ~LoadRocketDialog;  // Close the load dialog
        }
            ImGui::End();
    }

    // Dialog for displaying and managing rocket properties
    if (guiState & RocketPropertiesDialog && totalRocket) {
        if (ImGui::Begin("Built Rocket", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {            // Begin the "Built Rocket" window
            ImGui::SetWindowSize("Built Rocket", ImVec2(700, 400), ImGuiCond_FirstUseEver);     // Adjust size as needed

            // Main Rocket Properties
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Main Rocket Properties:");
            ImGui::Separator();
            ImGui::Text("Rocket Name: %s", totalRocket->getName().c_str());
            ImGui::Text("Total Mass: %.2f kg", totalRocket->getTotalMass());
            ImGui::Text("Total Structure Mass: %.2f kg", totalRocket->getStructureMass());
            ImGui::Text("Total Fuel Mass: %.2f kg", totalRocket->getFuelMass());
            ImGui::Text("DeltaV: %.2f km/s", totalRocket->getDeltaV());
            ImGui::Separator();
            ImGui::Spacing();

            // Stages Overview
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Stages Overview:");
            ImGui::Separator();
            if (ImGui::BeginTable("StagesTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Stage");
                ImGui::TableSetupColumn("Total Mass (kg)");
                ImGui::TableSetupColumn("Structure Mass (kg)");
                ImGui::TableSetupColumn("Fuel Mass (kg)");
                ImGui::TableSetupColumn("ISP (s)");
                ImGui::TableHeadersRow();

                std::queue<std::shared_ptr<RocketStage>> stages = totalRocket->getStageQueue();
                int stageCount = 1;
                while (!stages.empty()) {
                    auto stage = stages.front();
                    stages.pop();
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Stage %d", stageCount++);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.2f", stage->getTotalMass());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.2f", stage->getStructureMass());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%.2f", stage->getFuelMass());
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%.2f", stage->getI_sp());
                }
                ImGui::EndTable();
            }
            ImGui::Separator();
            ImGui::Spacing();

            // Rocket Performance Section
            std::mutex mtx; // Mutex for thread-safe updates to shared variables

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Rocket Performance Metrics:");
            ImGui::Separator();

            static bool inOrbit = false;
            static double altitude = 0.0;
            static double velocity = 0.0;
            static double latitude = 0.0;
            static double fuelToUse = 0.0;
            static double deltaVAvailable = 0.0;
            static bool calculationComplete = false;
            static bool calculationDisplay = false;
            static bool isCalculating = false;

            ImGui::Checkbox("In Orbit", &inOrbit);
            if (inOrbit) {
                ImGui::InputDouble("Current Altitude (km)", &altitude, 0.0, 0.0, "%.2f");
                ImGui::InputDouble("Velocity (km/s)", &velocity, 0.0, 0.0, "%.2f");
            }
            else {
                ImGui::InputDouble("Latitude of Launch Site (degrees)", &latitude, 0.0, 0.0, "%.2f");
                ImGui::InputDouble("Amount of Fuel to Use (kg)", &fuelToUse, 0.0, 0.0, "%.2f");
            }

            bool canCalculate = inOrbit ? (altitude >= 0 && velocity >= 0) :
                (latitude >= -90 && latitude <= 90 && fuelToUse >= 0 && fuelToUse <= totalRocket->getFuelMass());

            if (canCalculate && !isCalculating && ImGui::Button("Calculate", ImVec2(-1, 0))) {
                isCalculating = true;  // Flag to indicate calculation is in progress
                std::thread calculationThread([&]() {
                    double initialFuel = totalRocket->getFuelMass();  // Capture initial fuel for calculations
                    double structuralMass = totalRocket->getStructureMass();

                    if (inOrbit) {
                        double logTerm = std::log((initialFuel + structuralMass) / structuralMass);
                        deltaVAvailable = totalRocket->getDeltaV() * logTerm;
                    }
                    else {
                        const double PI = 3.141592653589793;
                        const double MU = 3.986e5; // Standard gravitational parameter
                        const double R_Earth = 6371.0; // Radius of Earth in kilometers
                        double v_circ = sqrt(MU / (R_Earth + altitude));
                        double v_rot = R_Earth * cos(latitude * PI / 180) * 2 * PI / 86400;
                        double v_total = v_circ + v_rot;
                        double logTerm = std::log((initialFuel + structuralMass) / structuralMass);
                        deltaVAvailable = totalRocket->getDeltaV() * logTerm;
                        deltaVAvailable = std::min(deltaVAvailable, v_total);
                    }

                    std::lock_guard<std::mutex> guard(mtx); // Ensure thread-safe updates
                    calculationComplete = true;
                    calculationDisplay = true;
                    isCalculating = false;
                    });
                calculationThread.detach(); // Detach the thread
            }

            if (isCalculating || !canCalculate) {
                ImGui::BeginDisabled();
                ImGui::Button("Calculate", ImVec2(-1, 0));
                ImGui::EndDisabled();
            }

            if (calculationDisplay) {
                ImGui::Separator();

                if (inOrbit) {
                    ImGui::Text("Current Altitude: %.2f km", altitude);
                    ImGui::Text("Velocity: %.2f km/s", velocity);
                    ImGui::Text("Delta V Available: %.2f km/s", deltaVAvailable);
                }
                else {
                    ImGui::Text("Latitude of Launch Site: %.2f degrees", latitude);
                    ImGui::Text("Amount of Fuel to Use: %.2f kg", fuelToUse);
                    ImGui::Text("Delta V Available for Launch: %.2f km/s", deltaVAvailable);
                }

                // Reset the flags after displaying the results
                calculationComplete = false; // This flag can be reset now
            }

            ImGui::Separator();

            // Dispalys
            if (ImGui::Button("Edit")) {
                guiState |= RocketBuilderDialog;                                                // Reopen rocket builder dialog
                guiState &= ~RocketPropertiesDialog;                                            // Close properties dialog temporarily
            }
            if (ImGui::Button("Destroy")) { 		                                            // Button to destroy the rocket
                while (!totalRocket->getStageQueue().empty()) {
                    totalRocket->detachStage();                                                 // Detach and deconstruct all stages
                }
                guiState &= ~RocketPropertiesDialog;                                            // Close properties dialog permanently
            }
            ImGui::End();
        }
    }





    //______________________________________________________________
    // ENDING

    // Rendering
    ImGui::Render();                                                                            // Render the ImGui
    int display_w, display_h;                                                                   // Display width and height
    glfwGetFramebufferSize(window, &display_w, &display_h);                                     // Get the framebuffer size
    glViewport(0, 0, display_w, display_h);                                                     // Set the viewport
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);                                                   // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);                                                               // Clear the color buffer
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());                                     // Render the ImGui draw data

    // Swap front and back buffers
    glfwSwapBuffers(window);

}

void GuiManager::RocketBuilder() {
    static int numStages = 1; 								                                    // Number of stages in the rocket
    static char name[1024] = ""; 							                                    // Buffer for the rocket name
    ImGui::InputText("Rocket Name", name, IM_ARRAYSIZE(name));                                  // Input for the rocket name
    totalRocket->setName(name);                                                                 // Set the rocket name
    ImGui::InputInt("Number of Stages", &numStages);
    numStages = std::max(1, numStages);                                                         // Ensure at least one stage

    // Ensure the rocket has the correct number of stages
    while (totalRocket->getStageQueue().size() < numStages) {
        totalRocket->addToRocket(new RocketStage(0, 0, 0, 0));                                     // Add stages as needed
    }
    while (totalRocket->getStageQueue().size() > numStages) {
        totalRocket->detachStage();                                                             // Remove stages in excess
    }

    if (ImGui::BeginTabBar("Stages Tab Bar")) {
        // Make a copy of the original queue for safe iteration
        auto tempQueue = totalRocket->getStageQueue();                                          // Copy the queue
        std::vector<RocketStage*> tempStages;                                                   // Temporary storage to preserve stages
        double structMass = 0.0;
        double fuelMass = 0.0;
        double isp = 0.0;

        while (!tempQueue.empty()) {                                                            // Copy the queue to the temporary vector
            RocketStage* stage = tempQueue.front().get();
            tempStages.push_back(stage);
            tempQueue.pop();
        }

        for (int i = 0; i < tempStages.size(); ++i) {
            std::string tabName = "Stage " + std::to_string(i + 1);                              // Tab name
            if (ImGui::BeginTabItem(tabName.c_str())) {                                          // Begin tab item

                // Get the mass values
                double structMass = tempStages[i]->getStructureMass();
                double fuelMass = tempStages[i]->getFuelMass();
                double isp = tempStages[i]->getI_sp();

                ImGui::Text("Structural Mass (kg):");
                if (ImGui::InputDouble("##StructuralMass", &structMass)) {
                    tempStages[i]->setStructureMass(structMass);                                 // Update the stage Structure mass
                }

                ImGui::Text("Fuel Mass (kg):");
                if (ImGui::InputDouble("##FuelMass", &fuelMass)) {
                    tempStages[i]->setFuelMass(fuelMass);                                        // Update the stage Fuel mass
                }

                ImGui::Text("Specific Impulse (s):");
                if (ImGui::InputDouble("##SpecificImpulse", &isp)) {
                    tempStages[i]->setI_sp(isp);                                                 // Update the stage Isp
                }

                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();                                                                      // End the tab bar
    }

    if (ImGui::Button("Cancel")) {
        // Clear the rocket and reset the dialog bit accordingly
        while (!totalRocket->getStageQueue().empty()) {
            totalRocket->detachStage();                                                          // Detach and deconstruct all stages
        }
        guiState &= ~RocketBuilderDialog;                                                        // Clear the Rocket Builder dialog bit
    }
}

void GuiManager::RocketLoader(const fs::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);  // Read the rocket name
    std::string rocketName = line;

    std::getline(file, line);  // Read the number of stages
    int numStages = std::stoi(line);

    totalRocket->setName(rocketName);

    for (int i = 0; i < numStages; ++i) {
        std::getline(file, line);
        double structMass = std::stod(line);

        std::getline(file, line);
        double fuelMass = std::stod(line);

        std::getline(file, line);
        double isp = std::stod(line);

        totalRocket->addToRocket(new RocketStage(structMass, fuelMass, isp));
    }

    file.close();
}