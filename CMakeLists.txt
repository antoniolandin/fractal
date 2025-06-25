cmake_minimum_required(VERSION 3.10)
project(fractal)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # Your project headers
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Eigen  # Eigen headers
)

# Collect all source files from src/ directory
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add your executable
add_executable(${PROJECT_NAME} 
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
