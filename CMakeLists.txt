# This CMakeLists is for the AERSP424 Final Project

cmake_minimum_required(VERSION 3.23)
project(RocketBuilder)
set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
else()
	message("OPENGL found")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include 
                    ${CMAKE_SOURCE_DIR}/include/GL 
		    ${CMAKE_SOURCE_DIR}/include/GLFW
		    ${CMAKE_SOURCE_DIR}/include/SDL
		    ${CMAKE_SOURCE_DIR}/include/glm
		    )

add_definitions(-DGLEW_STATIC) # static linking for glew only
set(GL_STATIC_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/glew32sd.lib;${CMAKE_SOURCE_DIR}/lib/glfw3.lib")
set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SDL2d.lib;${CMAKE_SOURCE_DIR}/lib/SDL2maind.lib")

add_executable(${CMAKE_PROJECT_NAME} # Source code file
    src/main.cpp
    src/satellite.h
    src/satellite.cpp
    src/orbit.h
    src/orbit.cpp
    src/booster.h
    src/booster.cpp
    src/Rocket.h
    src/Rocket.cpp
    lib/stb_image.h
    lib/tiny_obj_loader.h
    lib/imgui/imconfig.h
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui.h
    lib/imgui/imgui_impl_glfw.cpp
    lib/imgui/imgui_impl_glfw.h
    lib/imgui/imgui_impl_opengl3.cpp
    lib/imgui/imgui_impl_opengl3.h
    lib/imgui/imgui_impl_opengl3_loader.h
    lib/imgui/imgui_impl_sdl2.cpp
    lib/imgui/imgui_impl_sdl2.h
    lib/imgui/imgui_internal.h
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imstb_rectpack.h
    lib/imgui/imstb_textedit.h
    lib/imgui/imstb_truetype.h
)

target_include_directories(RocketBuilder PUBLIC
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(RocketBuilder
    ${OPENGL_gl_LIBRARY} ${GL_STATIC_LIBRARIES} ${SDL2_LIBRARIES}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# ^^^^^ Uncomment if we include shaders or assets

add_custom_command(TARGET RocketBuilder POST_BUILD          	# Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       	# which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/lib/SDL2d.dll"            		# <--this is in-file
        $<TARGET_FILE_DIR:RocketBuilder>)                   	# <--this is out-file path
		
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RocketBuilder)