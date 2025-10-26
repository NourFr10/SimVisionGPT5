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
)

target_link_libraries(SimVisionClone PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
