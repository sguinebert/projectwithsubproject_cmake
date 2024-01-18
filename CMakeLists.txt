# Minimum version of CMake
cmake_minimum_required(VERSION 3.3) # Specify the correct version

# Project name
project(project)

# Add subdirectories for each subproject
#if(WEBASSEMBLY_BUILD)
add_subdirectory(webassembly)


include(ExternalProject)
set(EMSCRIPTEN_TOOLCHAIN_FILE "C:/Users/gsylv/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
set(EMSCRIPTEN_PATH "C:/Users/gsylv/emsdk")

ExternalProject_Add(
    webassembly
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/webassembly"
    BINARY_DIR "${CMAKE_BINARY_DIR}/webassembly"
    CONFIGURE_COMMAND "${EMSCRIPTEN_PATH}/upstream/emscripten/emcmake" cmake -DWEBASSEMBLY_BUILD=ON <SOURCE_DIR>
    BUILD_COMMAND "${EMSCRIPTEN_PATH}/upstream/emscripten/emmake" make
    INSTALL_COMMAND ""
)

option(WEBASSEMBLY_BUILD "Build wasm and exe" OFF)

# ... add other subprojects as needed

# If your main project also has source files, you can add them here
add_executable(project main.cpp)
# ... add other source files and configurations for the main project

# Link against Qt modules or other libraries if necessary
# find_package(Qt5Core)
# target_link_libraries(YourProject Qt5::Core)
