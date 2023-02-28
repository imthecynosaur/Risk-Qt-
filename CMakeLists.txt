cmake_minimum_required(VERSION 3.14)

project(Risk LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(Risk
  main.cpp
  player.h
  player.cpp
  territory.h
  territory.cpp
  manager.h
  manager.cpp
  territories.h
  territories.cpp
)
target_link_libraries(Risk Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS Risk
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
