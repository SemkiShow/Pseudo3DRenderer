cmake_minimum_required(VERSION 3.28)
project(Pseudo3DRenderer)

if (UNIX)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Build GLFW Wayland support")
    set(GLFW_BUILD_X11 ON CACHE BOOL "Build GLFW X11 support")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)

# Turn off all warning-related errors when compiling third-party code
add_compile_options(-Wno-error)
message(STATUS "Downloading required libraries")

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/${RAYLIB_VERSION}.zip
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)

# Raygui
FetchContent_Declare(
    raygui
    URL https://github.com/raysan5/raygui/archive/${RAYGUI_VERSION}.zip
)
FetchContent_MakeAvailable(raygui)
add_subdirectory(${raygui_SOURCE_DIR}/projects/CMake)

add_compile_options(-Wall -Wextra)
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include/)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raygui)

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC 
        "-sASSERTIONS=1"
        "-sFORCE_FILESYSTEM"
        "-sALLOW_MEMORY_GROWTH"
        "-sASYNCIFY"
        "-sUSE_GLFW=3"
        "-sEXPORTED_RUNTIME_METHODS=['HEAPF32']"
        "--preload-file" "${CMAKE_SOURCE_DIR}/resources@resources"
        "--preload-file=${CMAKE_SOURCE_DIR}/settings.txt@settings.txt"
    )
    if ("${SHELL}" STREQUAL "Minimal")
        target_link_options(${PROJECT_NAME} PUBLIC "--shell-file" "${CMAKE_SOURCE_DIR}/src/shell_minimal.html")
    endif()
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif()
