cmake_minimum_required(VERSION 3.14)

project(TaskManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include_directories(${QT_INCLUDES})

#=============
# Add project files from dir
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#add_subdirectory(${PROJECT_NUM_FOLDER})

#=============
# Link library


add_executable(TaskManager
  main.cpp

  tcpserver.cpp
  dataoutputreader.cpp
  taskmanager.cpp
  serializer.cpp
  consoleinput.cpp
  fileinput.cpp

  tcpserver.h
  dataoutputreader.h
  taskmanager.h
  serializer.h
  consoleinput.h
  fileinput.h

  constants.h
)

target_link_libraries(TaskManager
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)

