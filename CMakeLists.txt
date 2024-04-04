# This CMakeLists is for the AERSP424 Final Project

cmake_minimum_required(VERSION 3.23)
project(RocketBuilder)
set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME} main.cpp) # Source code file
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}) # Directory of the header file = same as where this CMakeList is 

#target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/[INSERT LIBRARY NAME HERE])
# ^Uncomment this when we have some libraries