# Minimum version of CMake
cmake_minimum_required(VERSION 3.5) # Specify the correct version

# Project name
project(project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_definitions(-DBOOST_ASIO_HAS_IO_URING -DASIO_HAS_IO_URING)

# Add subdirectories for each subproject
add_subdirectory(webassembly EXCLUDE_FROM_ALL)

set(EMSDK_POSSIBLE_PATHS
    "/opt/emsdk"                     # Common path for system-wide installations in Linux
    "~/emsdk"                        # In user's home directory (Linux/macOS)
    "$ENV{HOME}/emsdk"               # Another way to refer to user's home directory
    "C:/emsdk"                       # Common path in Windows
    "$ENV{USERPROFILE}/Documents/emsdk" # User's Documents folder in Windows
)
set(EMSDK_PATH "EMSDK_PATH-NOTFOUND" CACHE PATH "Path to the Emscripten SDK")

foreach(PATH IN LISTS EMSDK_POSSIBLE_PATHS)
    if(EXISTS "${PATH}/.emscripten")
        set(EMSDK_PATH "${PATH}")
        break()  # Exit the loop once found
    endif()

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
    set(EMSCRIPTEN_PATH ${EMSDK_PATH})
endif()

include(ExternalProject)
#set(EMSCRIPTEN_PATH "/home/guinebert/emsdk")

# Get the directory of the Qt6CoreConfig.cmake file
get_filename_component(Qt6Core_CONFIG_DIR "${Qt6Core_DIR}" DIRECTORY)
# Assuming the standard directory layout, the Qt installation root will be two levels up
get_filename_component(QT_INSTALL_PREFIX "${Qt6Core_CONFIG_DIR}/../../.." ABSOLUTE)
set(WASM_PATH "${QT_INSTALL_PREFIX}/wasm_singlethread")

message(STATUS "Qt install path: ${QT_INSTALL_PREFIX} ${CMAKE_BINARY_DIR}")

# Path to a file that will act as a trigger for rebuilds
set(WEBASSEMBLY_TRIGGER "${CMAKE_BINARY_DIR}/webassembly_trigger")
set(HASH_FILE "${CMAKE_BINARY_DIR}/hash_file")


file(GLOB_RECURSE WEBASSEMBLY_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/webassembly/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/webassembly/*.h"
     # Add other file extensions as needed
)

#message(STATUS "source files: ${WEBASSEMBLY_SOURCES}")
# Compute hash for each file and concatenate
# set(COMBINED_HASH "")
# foreach(SOURCE_FILE IN LISTS WEBASSEMBLY_SOURCES)
#     file(SHA256 ${SOURCE_FILE} FILE_HASH)
#     string(APPEND COMBINED_HASH ${FILE_HASH})
# endforeach()

# # Check if the hash has changed
# if(EXISTS ${HASH_FILE})
#     file(READ ${HASH_FILE} LAST_HASH)
#     if(NOT LAST_HASH STREQUAL COMBINED_HASH)
#         set(HASH_CHANGED TRUE)
#         message(STATUS "Source changed, updating...")
#         file(WRITE ${HASH_FILE} ${COMBINED_HASH})
#         execute_process(COMMAND ${CMAKE_COMMAND} -E exit 0)
#     endif()
# else()
#     set(HASH_CHANGED TRUE)
#     message(STATUS "never executed, updating...")
#     file(WRITE ${HASH_FILE} ${COMBINED_HASH})
#     execute_process(COMMAND ${CMAKE_COMMAND} -E exit 0)
# endif()

# # Update the hash file
# if(NOT HASH_CHANGED)
#     message(STATUS "No source changes detected.")
#     execute_process(COMMAND ${CMAKE_COMMAND} -E exit 1)    # Add custom commands or steps to build the external project
#     # ...
# endif()


# Custom command to update the trigger file
# add_custom_command(
#     OUTPUT ${WEBASSEMBLY_TRIGGER}
#     COMMAND ${CMAKE_COMMAND} -E touch ${WEBASSEMBLY_TRIGGER}
#     DEPENDS ${WEBASSEMBLY_SOURCES}
#     COMMENT "Checking for WebAssembly source changes..."
# )

# add_custom_target(webassembly_trigger_target
#     ALL
#     DEPENDS ${HASH_FILE}
# )
#/home/guinebert/emsdk/node/14.18.2_64bit/bin:/home/guinebert/emsdk/upstream/emscripten:/home/guinebert/emsdk:/home/guinebert/emsdk/node/14.18.2_64bit/bin:/home/guinebert/emsdk/upstream/emscripten:/home/guinebert/emsdk:/home/guinebert/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/lib/postgresql/13/bin:/snap/bin:/home/guinebert/Qt/Tools/Ninja
ExternalProject_Add(
    webassembly
    #DEPENDS webassembly_trigger_target
    #UPDATE_COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/check_for_changes.cmake"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/webassembly"
    BINARY_DIR "${CMAKE_BINARY_DIR}/webassembly"
    CONFIGURE_COMMAND EMSDK=${EMSCRIPTEN_PATH} ${WASM_PATH}/bin/qt-cmake #--debug-find-pkg=Qt6Core --debug-find-pkg=WrapRt #--trace-expand

    -DQT_CHAINLOAD_TOOLCHAIN_FILE=${EMSCRIPTEN_PATH}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
    -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${WASM_PATH}/lib/cmake/Qt6/qt.toolchain.cmake

    -DCMAKE_BUILD_TYPE:STRING=Release
    -DCMAKE_COLOR_DIAGNOSTICS:BOOL=1
    -DCMAKE_CROSSCOMPILING_EMULATOR:FILEPATH=/usr/bin/nodejs
    -DCMAKE_CXX_FLAGS_INIT:STRING=-DQT_QML_DEBUG
    -DCMAKE_CXX_OUTPUT_EXTENSION:STRING=.o
    -DCMAKE_C_COMPILER:FILEPATH=${EMSCRIPTEN_PATH}/upstream/emscripten/emcc
    -DCMAKE_C_COMPILER_AR:FILEPATH=${EMSCRIPTEN_PATH}/upstream/emscripten/emar
    -DCMAKE_C_COMPILER_RANLIB:FILEPATH=${EMSCRIPTEN_PATH}/upstream/emscripten/emranlib
    -DCMAKE_C_OUTPUT_EXTENSION:STRING=
    -DCMAKE_GENERATOR:STRING=Ninja
    -DCMAKE_INSTALL_PREFIX:PATH=${EMSCRIPTEN_PATH}/upstream/emscripten/cache/sysroot
    -DCMAKE_PREFIX_PATH:PATH=${WASM_PATH}
    -DEMSCRIPTEN:BOOL=1
    -DEMSCRIPTEN_FORCE_COMPILERS:BOOL=ON
    -DEMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES:BOOL=OFF
    -DLIBRT:FILEPATH=LIBRT-NOTFOUND
    -DNODE_JS_EXECUTABLE:FILEPATH=/usr/bin/nodejs
    -DQT_ADDITIONAL_HOST_PACKAGES_PREFIX_PATH:STRING=
    -DQT_ADDITIONAL_PACKAGES_PREFIX_PATH:STRING=
    -DQT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP:BOOL=OFF
    -DQT_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6
    -DQT_HOST_PATH:PATH=${QT_INSTALL_PREFIX}/gcc_64
    -DQT_HOST_PATH_CMAKE_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake
    -DQT_QMAKE_EXECUTABLE:FILEPATH=${WASM_PATH}/bin/qmake
    -DQT_QMAKE_TARGET_MKSPEC:STRING=wasm-emscripten
    -DQt6BodymovinPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BodymovinPrivate
    -DQt6BundledFreetype_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledFreetype
    -DQt6BundledGlslang_Glslang_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledGlslang_Glslang
    -DQt6BundledGlslang_Oglcompiler_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledGlslang_Oglcompiler
    -DQt6BundledGlslang_Osdependent_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledGlslang_Osdependent
    -DQt6BundledGlslang_Spirv_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledGlslang_Spirv
    -DQt6BundledHarfbuzz_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledHarfbuzz
    -DQt6BundledLibjpeg_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledLibjpeg
    -DQt6BundledLibpng_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledLibpng
    -DQt6BundledOpenwnn_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledOpenwnn
    -DQt6BundledPcre2_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledPcre2
    -DQt6BundledPhysX_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledPhysX
    -DQt6BundledResonanceAudio_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledResonanceAudio
    -DQt6BundledSpirv_Cross_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledSpirv_Cross
    -DQt6BundledTcime_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledTcime
    -DQt6BundledZLIB_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6BundledZLIB
    -DQt6ChartsQml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6ChartsQml
    -DQt6Charts_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Charts
    -DQt6CoreTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6CoreTools
    -DQt6Core_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Core
    -DQt6DataVisualizationQml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6DataVisualizationQml
    -DQt6DataVisualization_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6DataVisualization
    -DQt6GuiTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6GuiTools
    -DQt6Gui_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Gui
    -DQt6HostInfo_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6HostInfo
    -DQt6InsightTrackerQml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6InsightTrackerQml
    -DQt6InsightTracker_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6InsightTracker
    -DQt6LabsAnimation_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6LabsAnimation
    -DQt6LabsFolderListModel_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6LabsFolderListModel
    -DQt6LabsQmlModels_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6LabsQmlModels
    -DQt6LabsSettings_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6LabsSettings
    -DQt6LabsWavefrontMesh_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6LabsWavefrontMesh
    -DQt6MultimediaQuickPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6MultimediaQuickPrivate
    -DQt6Multimedia_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Multimedia
    -DQt6Network_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Network
    -DQt6OpenGLWidgets_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6OpenGLWidgets
    -DQt6OpenGL_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6OpenGL
    -DQt6QmlCompilerPlusPrivate_DIR:PATH=Qt6QmlCompilerPlusPrivate_DIR-NOTFOUND
    -DQt6QmlCore_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QmlCore
    -DQt6QmlIntegration_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QmlIntegration
    -DQt6QmlLocalStorage_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QmlLocalStorage
    -DQt6QmlModels_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QmlModels
    -DQt6QmlTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6QmlTools
    -DQt6Qml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Qml
    -DQt6Quick3DAssetImport_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DAssetImport
    -DQt6Quick3DAssetUtils_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DAssetUtils
    -DQt6Quick3DEffects_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DEffects
    -DQt6Quick3DHelpersImpl_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DHelpersImpl
    -DQt6Quick3DHelpers_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DHelpers
    -DQt6Quick3DParticleEffects_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DParticleEffects
    -DQt6Quick3DParticles_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DParticles
    -DQt6Quick3DPhysicsHelpers_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DPhysicsHelpers
    -DQt6Quick3DPhysics_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DPhysics
    -DQt6Quick3DRuntimeRender_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DRuntimeRender
    -DQt6Quick3DSpatialAudioPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DSpatialAudioPrivate
    -DQt6Quick3DTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6Quick3DTools
    -DQt6Quick3DUtils_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3DUtils
    -DQt6Quick3D_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick3D
    -DQt6QuickControls2Impl_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickControls2Impl
    -DQt6QuickControls2_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickControls2
    -DQt6QuickControlsTestUtilsPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickControlsTestUtilsPrivate
    -DQt6QuickDialogs2QuickImpl_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickDialogs2QuickImpl
    -DQt6QuickDialogs2Utils_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickDialogs2Utils
    -DQt6QuickDialogs2_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickDialogs2
    -DQt6QuickEffectsPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickEffectsPrivate
    -DQt6QuickLayouts_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickLayouts
    -DQt6QuickParticlesPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickParticlesPrivate
    -DQt6QuickShapesPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickShapesPrivate
    -DQt6QuickTemplates2_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickTemplates2
    -DQt6QuickTestUtilsPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickTestUtilsPrivate
    -DQt6QuickTest_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickTest
    -DQt6QuickTimeline_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6QuickTimeline
    -DQt6Quick_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Quick
    -DQt6ScxmlQml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6ScxmlQml
    -DQt6ScxmlTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6ScxmlTools
    -DQt6Scxml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Scxml
    -DQt6ShaderToolsTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6ShaderToolsTools
    -DQt6ShaderTools_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6ShaderTools
    -DQt6SpatialAudio_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6SpatialAudio
    -DQt6Sql_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Sql
    -DQt6StateMachineQml_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6StateMachineQml
    -DQt6StateMachine_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6StateMachine
    -DQt6Svg_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Svg
    -DQt6Test_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Test
    -DQt6TextToSpeech_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6TextToSpeech
    -DQt6VirtualKeyboard_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6VirtualKeyboard
    -DQt6WebChannel_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6WebChannel
    -DQt6WebSockets_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6WebSockets
    -DQt6WidgetsTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6WidgetsTools
    -DQt6Widgets_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6Widgets
    -DQt6ZlibPrivate_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6ZlibPrivate
    -DQt6_DIR:PATH=${WASM_PATH}/lib/cmake/Qt6

    ${CMAKE_SOURCE_DIR}/webassembly
    BUILD_COMMAND "${EMSCRIPTEN_PATH}/upstream/emscripten/emmake" ninja -j32
    INSTALL_COMMAND ""
    #BUILD_ALWAYS OFF
)

option(WEBASSEMBLY_BUILD "Build wasm and exe" OFF)

#add_dependencies(webassembly webassembly_trigger_target)

# Ensure that the external project depends on the trigger
#add_dependencies(webassembly ${WEBASSEMBLY_TRIGGER})

# ... add other subprojects as needed
#list(APPEND CMAKE_PREFIX_PATH "/custom/path")
#or find_package(SomeLibrary REQUIRED PATHS "/custom/path")
find_package(wt REQUIRED)
find_library(WT_LIBRARY NAMES wt)
# target_link_libraries(project PRIVATE Wt)

message(STATUS "Wt include directory: ${WT_INCLUDE_DIR}")
message(STATUS "Wt libraries: ${WT_LIBRARY}")


# If your main project also has source files, you can add them here
add_executable(project main.cpp)
# ... add other source files and configurations for the main project

# Link against libraries if necessary
target_link_libraries(project PRIVATE Wt::Wt)

# target_link_libraries(project PRIVATE ${WT_LIBRARY})
# target_include_directories(project PRIVATE ${Wt_INCLUDE_DIRS})

find_package(ZLIB)

find_package(OpenSSL)

TARGET_LINK_LIBRARIES(project PRIVATE ${OPENSSL_LIBRARIES})
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
IF(WIN32)
  TARGET_LINK_LIBRARIES(project PRIVATE Crypt32.lib)
ENDIF(WIN32)

TARGET_INCLUDE_DIRECTORIES(project PRIVATE ${ZLIB_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(project PRIVATE ${ZLIB_LIBRARIES})
