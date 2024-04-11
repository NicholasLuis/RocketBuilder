// This file contains all global variables/functions
# include <../src/booster.h>
# include <../src/orbit.h>
# include <../src/satellite.h>
# include <../src/GuiManager.h>

# include <GLFW/glfw3.h>
# include <../lib/imgui/imgui.h>
# include <../lib/imgui/imgui_impl_glfw.h>
# include <../lib/imgui/imgui_impl_opengl3.h>

# include <iostream>
# include <string>
# include <filesystem>
# include <optional>
# include <regex>
# include <thread>

// global gravitational constant
#ifndef G_H
#define G_H


double g = 9.81;


#endif