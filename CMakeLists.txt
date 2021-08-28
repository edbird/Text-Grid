cmake_minimum_required(VERSION 3.0)

project(textgrid VERSION 1.0)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(textgrid main.cpp)
target_link_libraries(textgrid ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})

configure_file(version.hpp.in version.hpp)
target_include_directories(textgrid PUBLIC "${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
