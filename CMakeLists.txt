cmake_minimum_required(VERSION 3.17)
project(nokia_csv)

set(CMAKE_CXX_STANDARD 14)

add_executable(nokia_csv src/main.cpp src/CSV_reader.cpp src/CSV_reader.h)