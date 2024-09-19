cmake_minimum_required(VERSION 3.10)
project(RATP_PROJET)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the current directory to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find all cpp files in the current directory
file(GLOB SOURCES "*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set the working directory for the target to the source directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Copy CSV files to the build directory
file(GLOB CSV_FILES "data/*.csv")
foreach(CSV_FILE ${CSV_FILES})
    configure_file(${CSV_FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()