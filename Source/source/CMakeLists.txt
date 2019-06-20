cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
project (LORIS-GPS)
include_directories( minmea-master )
add_executable(Loris-GPS GPS.cpp GPS.hpp)

