# Minimum version of CMake
cmake_minimum_required(VERSION 3.5) # Specify the correct version

# Project name
project(project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_definitions(-DBOOST_ASIO_HAS_IO_URING -DASIO_HAS_IO_URING)

# Add subdirectories for each subproject
add_subdirectory(webassembly)

include(ExternalProject)
set(EMSCRIPTEN_PATH "/home/guinebert/emsdk")

# Get the directory of the Qt6CoreConfig.cmake file
get_filename_component(Qt6Core_CONFIG_DIR "${Qt6Core_DIR}" DIRECTORY)
# Assuming the standard directory layout, the Qt installation root will be two levels up
get_filename_component(QT_INSTALL_PREFIX "${Qt6Core_CONFIG_DIR}/../../.." ABSOLUTE)

message(STATUS "Qt install path: ${QT_INSTALL_PREFIX}")

ExternalProject_Add(
    webassembly
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/webassembly"
    BINARY_DIR "${CMAKE_BINARY_DIR}/webassembly"
    CONFIGURE_COMMAND "${EMSCRIPTEN_PATH}/upstream/emscripten/emcmake" cmake
        -DCMAKE_FIND_DEBUG_MODE=TRUE
        -DCMAKE_FIND_DEBUG_MODE=TRUE
        -DCMAKE_BUILD_TYPE=Release
        -DQT_ADDITIONAL_PACKAGES_PREFIX_PATH=${QT_INSTALL_PREFIX}/wasm_singlethread
        -DQT_QMAKE_TARGET_MKSPEC:STRING=wasm-emscripten
        -DQt6CoreTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6CoreTools
        -DQt6Core_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Core
        -DQt6BundledZLIB_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledZLIB
        -DQt6ZlibPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6ZlibPrivate
        -DQt6BodymovinPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BodymovinPrivate
        -DQt6BundledFreetype_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledFreetype
        -DQt6BundledGlslang_Glslang_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledGlslang_Glslang
        -DQt6BundledGlslang_Oglcompiler_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledGlslang_Oglcompiler
        -DQt6BundledGlslang_Osdependent_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledGlslang_Osdependent
        -DQt6BundledGlslang_Spirv_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledGlslang_Spirv
        -DQt6BundledHarfbuzz_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledHarfbuzz
        -DQt6BundledLibjpeg_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledLibjpeg
        -DQt6BundledLibpng_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledLibpng
        -DQt6BundledOpenwnn_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledOpenwnn
        -DQt6BundledPcre2_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledPcre2
        -DQt6BundledPhysX_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledPhysX
        -DQt6BundledResonanceAudio_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledResonanceAudio
        -DQt6BundledSpirv_Cross_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledSpirv_Cross
        -DQt6BundledTcime_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledTcime
        -DQt6BundledZLIB_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6BundledZLIB
        -DQt6ChartsQml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6ChartsQml
        -DQt6Charts_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Charts
        -DQt6CoreTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6CoreTools
        -DQt6Core_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Core
        -DQt6DataVisualizationQml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6DataVisualizationQml
        -DQt6DataVisualization_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6DataVisualization
        -DQt6GuiTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6GuiTools
        -DQt6Gui_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Gui
        -DQt6HostInfo_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6HostInfo
        -DQt6InsightTrackerQml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6InsightTrackerQml
        -DQt6InsightTracker_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6InsightTracker
        -DQt6LabsAnimation_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6LabsAnimation
        -DQt6LabsFolderListModel_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6LabsFolderListModel
        -DQt6LabsQmlModels_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6LabsQmlModels
        -DQt6LabsSettings_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6LabsSettings
        -DQt6LabsWavefrontMesh_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6LabsWavefrontMesh
        -DQt6MultimediaQuickPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6MultimediaQuickPrivate
        -DQt6Multimedia_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Multimedia
        -DQt6Network_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Network
        -DQt6OpenGLWidgets_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6OpenGLWidgets
        -DQt6OpenGL_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6OpenGL
        -DQt6QmlCompilerPlusPrivate_DIR:PATH=Qt6QmlCompilerPlusPrivate_DIR-NOTFOUND
        -DQt6QmlCore_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QmlCore
        -DQt6QmlIntegration_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QmlIntegration
        -DQt6QmlLocalStorage_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QmlLocalStorage
        -DQt6QmlModels_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QmlModels
        -DQt6QmlTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6QmlTools
        -DQt6Qml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Qml
        -DQt6Quick3DAssetImport_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DAssetImport
        -DQt6Quick3DAssetUtils_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DAssetUtils
        -DQt6Quick3DEffects_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DEffects
        -DQt6Quick3DHelpersImpl_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DHelpersImpl
        -DQt6Quick3DHelpers_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DHelpers
        -DQt6Quick3DParticleEffects_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DParticleEffects
        -DQt6Quick3DParticles_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DParticles
        -DQt6Quick3DPhysicsHelpers_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DPhysicsHelpers
        -DQt6Quick3DPhysics_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DPhysics
        -DQt6Quick3DRuntimeRender_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DRuntimeRender
        -DQt6Quick3DSpatialAudioPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DSpatialAudioPrivate
        -DQt6Quick3DTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6Quick3DTools
        -DQt6Quick3DUtils_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3DUtils
        -DQt6Quick3D_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick3D
        -DQt6QuickControls2Impl_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickControls2Impl
        -DQt6QuickControls2_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickControls2
        -DQt6QuickControlsTestUtilsPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickControlsTestUtilsPrivate
        -DQt6QuickDialogs2QuickImpl_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickDialogs2QuickImpl
        -DQt6QuickDialogs2Utils_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickDialogs2Utils
        -DQt6QuickDialogs2_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickDialogs2
        -DQt6QuickEffectsPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickEffectsPrivate
        -DQt6QuickLayouts_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickLayouts
        -DQt6QuickParticlesPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickParticlesPrivate
        -DQt6QuickShapesPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickShapesPrivate
        -DQt6QuickTemplates2_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickTemplates2
        -DQt6QuickTestUtilsPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickTestUtilsPrivate
        -DQt6QuickTest_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickTest
        -DQt6QuickTimeline_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6QuickTimeline
        -DQt6Quick_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Quick
        -DQt6ScxmlQml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6ScxmlQml
        -DQt6ScxmlTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6ScxmlTools
        -DQt6Scxml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Scxml
        -DQt6ShaderToolsTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6ShaderToolsTools
        -DQt6ShaderTools_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6ShaderTools
        -DQt6SpatialAudio_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6SpatialAudio
        -DQt6Sql_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Sql
        -DQt6StateMachineQml_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6StateMachineQml
        -DQt6StateMachine_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6StateMachine
        -DQt6Svg_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Svg
        -DQt6Test_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Test
        -DQt6TextToSpeech_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6TextToSpeech
        -DQt6VirtualKeyboard_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6VirtualKeyboard
        -DQt6WebChannel_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6WebChannel
        -DQt6WebSockets_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6WebSockets
        -DQt6WidgetsTools_DIR:PATH=${QT_INSTALL_PREFIX}/gcc_64/lib/cmake/Qt6WidgetsTools
        -DQt6Widgets_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6Widgets
        -DQt6ZlibPrivate_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6ZlibPrivate
        -DQt6_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6
        -DQt6_DIR:PATH=${QT_INSTALL_PREFIX}/wasm_singlethread/lib/cmake/Qt6
        -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN_PATH}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
        -DQT_HOST_PATH=${QT_INSTALL_PREFIX}/gcc_64
        -DCMAKE_PREFIX_PATH=${QT_INSTALL_PREFIX}/wasm_singlethread
        -DWEBASSEMBLY_BUILD=ON
        -DEMSCRIPTEN_PATH=${EMSCRIPTEN_PATH}
        ${CMAKE_SOURCE_DIR}/webassembly
    BUILD_COMMAND "${EMSCRIPTEN_PATH}/upstream/emscripten/emmake" make -j32
    INSTALL_COMMAND ""
)

option(WEBASSEMBLY_BUILD "Build wasm and exe" OFF)



#message(STATUS "Qt install path: ${QT_INSTALL_PREFIX}")



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
