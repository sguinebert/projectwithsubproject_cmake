cmake_minimum_required(VERSION 3.5)

project(WebAssembly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

set(PROJECT_SOURCES
        main.cpp JsBridge.h JsBridge.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WebAssembly
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WebAssembly APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WebAssembly SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WebAssembly
          ${PROJECT_SOURCES}
        )
    endif()
endif()

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
    target_include_directories(WebAssembly PRIVATE ${EMSDK_PATH}/upstream/emscripten/cache/sysroot/include)
endif()


target_link_libraries(WebAssembly
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WebAssembly)
endif()
set_target_properties(WebAssembly PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WebAssembly
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(WebAssembly)
    qt_finalize_executable(WebAssembly)
endif()



# cmake_minimum_required(VERSION 3.5) # Specify your version

# project(WebAssembly VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt6 REQUIRED COMPONENTS Core Quick)

# message(STATUS "Qt6_FOUND: ${Qt6_FOUND}")

# set(EMSDK_POSSIBLE_PATHS
#     "/opt/emsdk"                     # Common path for system-wide installations in Linux
#     "~/emsdk"                        # In user's home directory (Linux/macOS)
#     "$ENV{HOME}/emsdk"               # Another way to refer to user's home directory
#     "C:/emsdk"                       # Common path in Windows
#     "$ENV{USERPROFILE}/Documents/emsdk" # User's Documents folder in Windows
# )
# set(EMSDK_PATH "EMSDK_PATH-NOTFOUND" CACHE PATH "Path to the Emscripten SDK")

# foreach(PATH IN LISTS EMSDK_POSSIBLE_PATHS)
#     # Replace 'emsdk_env.sh' with a file you expect to find in the emsdk directory
#     if(EXISTS "${PATH}/emsdk_env.sh")
#         set(EMSDK_PATH "${PATH}")
#         break()  # Exit the loop once found
#     endif()
# endforeach()

# if(EMSDK_PATH STREQUAL "EMSDK_PATH-NOTFOUND")
#     message(FATAL_ERROR "Emscripten SDK not found in any of the specified paths.")
# else()
#     message(STATUS "Found Emscripten SDK at: ${EMSDK_PATH}")
#     target_include_directories(WebAssembly PRIVATE ${EMSDK_PATH}/upstream/emscripten/cache/sysroot/include)
# endif()

# qt_standard_project_setup()

# # Set up the sources, include directories, and any necessary flags
# set(PROJECT_SOURCES
#         main.cpp
#         qml.qrc
# )

    # qt_add_executable(WebAssembly
    #     MANUAL_FINALIZATION
    #     ${PROJECT_SOURCES}
    # )


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
# set_target_properties(WebAssembly PROPERTIES
# #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptest_qtquick
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# target_link_libraries(WebAssembly
#     PRIVATE Qt6::Quick
# )

# qt_add_resources(WebAssembly "resources"
#     PREFIX "/"
#     FILES qml.qrc)



# include(GNUInstallDirs)
# install(TARGETS WebAssembly
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )


# qt_add_qml_module(WebAssembly
#     URI WebAssembly
#     VERSION 1.0
#     QML_FILES main.qml
#     SOURCES JsBridge.cpp JsBridge.h
# )

# if(NOT WEBASSEMBLY_BUILD)
# set_target_properties(WebAssembly PROPERTIES EXCLUDE_FROM_ALL TRUE)
# endif()

# ... more configurations
