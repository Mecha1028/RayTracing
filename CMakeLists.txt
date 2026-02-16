cmake_minimum_required(VERSION 3.26.0)
project(RayTrace VERSION 1.0.0)

# Only list .cpp files here (headers aren't compiled directly)
add_executable(RayTrace src/main.cpp)

# Tell the compiler to look in the src folder for headers
target_include_directories(RayTrace PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)