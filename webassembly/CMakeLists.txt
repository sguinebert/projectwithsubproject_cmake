cmake_minimum_required(VERSION 3.5) # Specify your version

project(WebAssembly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

set(EMSDK_POSSIBLE_PATHS
    "/opt/emsdk"                     # Common path for system-wide installations in Linux
    "~/emsdk"                        # In user's home directory (Linux/macOS)
    "$ENV{HOME}/emsdk"               # Another way to refer to user's home directory
    "C:/emsdk"                       # Common path in Windows
    "$ENV{USERPROFILE}/Documents/emsdk" # User's Documents folder in Windows
)
set(EMSDK_PATH "EMSDK_PATH-NOTFOUND" CACHE PATH "Path to the Emscripten SDK")

foreach(PATH IN LISTS EMSDK_POSSIBLE_PATHS)
    # Replace 'emsdk_env.sh' with a file you expect to find in the emsdk directory
    if(EXISTS "${PATH}/emsdk_env.sh")
        set(EMSDK_PATH "${PATH}")
        break()  # Exit the loop once found
    endif()
endforeach()

if(EMSDK_PATH STREQUAL "EMSDK_PATH-NOTFOUND")
    message(FATAL_ERROR "Emscripten SDK not found in any of the specified paths.")
else()
    message(STATUS "Found Emscripten SDK at: ${EMSDK_PATH}")
endif()

qt_standard_project_setup()

# Set up the sources, include directories, and any necessary flags
add_executable(WebAssembly main.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(WebAssembly PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_qtquick
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(WebAssembly
    PRIVATE Qt6::Quick
)

qt_add_resources(WebAssembly "resources"
    PREFIX "/"
    FILES qml.qrc)


target_include_directories(WebAssembly PRIVATE ${EMSDK_PATH}/upstream/emscripten/cache/sysroot/include)

include(GNUInstallDirs)
install(TARGETS WebAssembly
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


qt_add_qml_module(WebAssembly
    URI WebAssembly
    VERSION 1.0
    QML_FILES main.qml
    SOURCES JsBridge.cpp JsBridge.h
)

if(NOT WEBASSEMBLY_BUILD)
set_target_properties(WebAssembly PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# ... more configurations
