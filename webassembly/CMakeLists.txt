cmake_minimum_required(VERSION 3.5) # Specify your version

project(WebAssembly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(QT_POSSIBLE_PATHS
#     "/opt/Qt"                     # Common path for system-wide installations in Linux
#     "~/Qt"                        # In user's home directory (Linux/macOS)
#     "$ENV{HOME}/Qt"               # Another way to refer to user's home directory
#     "C:/Qt"                       # Common path in Windows
#     "$ENV{USERPROFILE}/Documents/Qt" # User's Documents folder in Windows
# )
# # Convert the list to a string with the platform-specific path separator
# string(REPLACE ";" ":" QT_PATHS_STR "${QT_POSSIBLE_PATHS}")
# # Set CMAKE_PREFIX_PATH to include these paths

#find_package(Qt6 REQUIRED COMPONENTS Core Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)

message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

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

include(GNUInstallDirs)
install(TARGETS WebAssembly
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


qt_add_qml_module(WebAssembly
    URI WebAssembly
    VERSION 1.0
    QML_FILES Main.qml
)

if(NOT WEBASSEMBLY_BUILD)
set_target_properties(WebAssembly PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

# ... more configurations
