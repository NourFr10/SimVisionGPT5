cmake_minimum_required(VERSION 3.16)
project(SimVisionClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

add_executable(SimVisionClone
    src/main.cpp

    src/MainWindow.cpp
    src/MainWindow.h

    src/WaveformView.cpp
    src/WaveformView.h

    src/core/Signal.h
    src/core/WaveformDatabase.cpp
    src/core/WaveformDatabase.h
    src/core/VCDParser.cpp
    src/core/VCDParser.h

    src/gui/HierarchyDock.cpp
    src/gui/HierarchyDock.h
)

target_include_directories(SimVisionClone PRIVATE src)

target_link_libraries(SimVisionClone PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
