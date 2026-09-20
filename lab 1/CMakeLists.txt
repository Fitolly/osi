cmake_minimum_required(VERSION 3.10)
project(lab1 CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Creator Creator.cpp)
add_executable(Reporter Reporter.cpp)
add_executable(Main Main.cpp)

add_dependencies(Main Creator Reporter)