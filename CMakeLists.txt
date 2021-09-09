cmake_minimum_required(VERSION 3.7)

project(textgrid VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_MODULE_PATH /home/ecb/Worlds-Best-Text-Editor-3/Text-Grid)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(textgrid ../Text-Graphics-Lib ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

# build text graphics libarary first
include_directories(textgraphicslib ../Text-Graphics-Lib ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
    # TODO: need a better way of managing these files: cmake set source directory or something similar?
set(TEXTGRAPHICSLIB_SOURCE_FILES ../Text-Graphics-Lib/fc_helper.cpp ../Text-Graphics-Lib/sdlfontmanager.cpp ../Text-Graphics-Lib/sdlfontmanager_fontmanagerinitsuccess.cpp ../Text-Graphics-Lib/sdlfonttexturemanager.cpp ../Text-Graphics-Lib/fc_helper.cpp ../Text-Graphics-Lib/sdlhelper.cpp)
add_library(textgraphicslib ${TEXTGRAPHICSLIB_SOURCE_FILES})
target_link_libraries(textgraphicslib PUBLIC SDL2::SDL2)
target_include_directories(textgraphicslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# build the executable test program last
set(SOURCE_FILES main.cpp)
add_executable(textgridexe ${SOURCE_FILES})
target_link_libraries(textgridexe ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})

configure_file(version.hpp.in version.hpp)
target_include_directories(textgridexe PUBLIC "${PROJECT_BINARY_DIR}")
