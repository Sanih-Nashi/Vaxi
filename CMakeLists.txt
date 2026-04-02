cmake_minimum_required(VERSION 3.16)

project(vaxi)

message(STATUS "Building ${PROJECT_NAME}")

message(STATUS "File: ${CPP_FILE}")

# Set the C++ version to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Define your target
add_executable(${PROJECT_NAME} src/main.cpp src/init.cpp src/input.cpp src/parse_and_execute.cpp src/raw_mode.cpp src/definitions.cpp)

# Apply the compile options (target-specific)
target_compile_options(${PROJECT_NAME} PRIVATE 
    -fdiagnostics-color=always  # Enable colored diagnostics (useful for readability)
    -O3
    -std=c++23
)

# Optionally link the AddressSanitizer runtime (needed for AddressSanitizer to work)
