cmake_minimum_required(VERSION 3.2) # Specify your version

set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_PATH}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
project(WebAssemblySubproject)


# Set up the sources, include directories, and any necessary flags
add_executable(WebAssemblyExecutable main.cpp
)

if(NOT WEBASSEMBLY_BUILD)
set_target_properties(WebAssemblyExecutable PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()


# ... more configurations
