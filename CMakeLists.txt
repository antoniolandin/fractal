cmake_minimum_required(VERSION 3.14)
project(fractal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Salida de ejecutable
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)

# Download SFML from github
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries")

# Descarga e incluye SFML en la build
FetchContent_MakeAvailable(sfml)

# Directorios locales
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/Eigen
)

# Archivos fuente
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Ejecutable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Vincular SFM
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
)

# Post-build: symlink compile_commands.json
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

# Clean and run targets
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/compile_commands.json
)

add_custom_target(run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
