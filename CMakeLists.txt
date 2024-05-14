cmake_minimum_required(VERSION 3.14)

project(Node LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(Node
  main.cpp
  node.h node.cpp
  node_info.h
  discovery_service.h discovery_service.cpp
  tnode_ethernet.h tnode_ethernet.cpp
  data_storage_processing.h data_storage_processing.cpp
  power_meter.h power_meter.cpp
  commands.h
  loger.h
  compute_core.h compute_core.cpp
  element.h element.cpp
  nodeserializer.cpp nodeserializer.h
  tcpserver.cpp
)
target_link_libraries(Node Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS Node
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
