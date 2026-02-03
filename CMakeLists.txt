cmake_minimum_required(VERSION 3.26.0)
project(RayTrace VERSION 1.0.0)

# adding source files to our exectuable programs
add_executable(RayTrace src/main.cpp)

# specify include directories
target_include_directories(RayTrace PRIVATE include )