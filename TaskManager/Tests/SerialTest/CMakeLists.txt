cmake_minimum_required(VERSION 3.5)

project(dataInpTest LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Test REQUIRED)
find_package(Qt5 COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dataInpTest tst_datainptest.cpp ../../serializer.cpp)
add_test(NAME dataInpTest COMMAND dataInpTest)

target_link_libraries(dataInpTest PRIVATE Qt5::Test)

