cmake_minimum_required(VERSION 3.28)
project(fractal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Salida de ejecutable
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Descargar thread-pool
FetchContent_Declare(
    thread-pool
    GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

# Descargar Eigen
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
)

# Descargar Root-Finder
FetchContent_Declare(
    root-finder
    GIT_REPOSITORY https://github.com/ZhepeiWang/Root-Finder.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
# Habilitar todas las descargas
FetchContent_MakeAvailable(SFML thread-pool Eigen root-finder)

# Directorios locales e includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Eigen_SOURCE_DIR}
    ${thread-pool_SOURCE_DIR}/include
    ${root-finder_SOURCE_DIR}/root_finder/include/root_finder
)

# Archivos fuente
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Ejecutable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)


# Vincular SFML
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    Eigen3::Eigen
)

# Compilar solo en C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

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
