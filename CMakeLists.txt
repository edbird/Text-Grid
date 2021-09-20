cmake_minimum_required(VERSION 3.7)

project(textgrid VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE DEBUG)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_MODULE_PATH /home/ecb/Worlds-Best-Text-Editor-3/Text-Grid)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(textgrid ../Text-Graphics-Lib ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

# build text graphics libarary first
include_directories(textgraphicslib ../Text-Graphics-Lib ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
    # TODO: need a better way of managing these files: cmake set source directory or something similar?

set(SOURCE_FILES main.cpp textgrid_print.cpp textgrid.cpp filltextgridfromtextbuffer.cpp)
set(TEXTGRAPHICSLIB_SOURCE_FILES ../Text-Graphics-Lib/fc_helper.cpp ../Text-Graphics-Lib/sdlfontmanager.cpp ../Text-Graphics-Lib/sdlfontmanager_fontmanagerinitsuccess.cpp ../Text-Graphics-Lib/sdlfonttexture.cpp ../Text-Graphics-Lib/sdlhelper.cpp ../Text-Graphics-Lib/tty_write_functions.cpp)
#set(TEXTGRAPHICSLIB_SOURCE_FILES fc_helper.cpp sdlfontmanager.cpp sdlfontmanager_fontmanagerinitsuccess.cpp sdlfonttexture.cpp sdlhelper.cpp tty_write_functions.cpp)

add_library(textgraphicslib ${TEXTGRAPHICSLIB_SOURCE_FILES})
target_link_libraries(textgraphicslib PUBLIC SDL2::SDL2)
target_include_directories(textgraphicslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(textgridexe ${SOURCE_FILES})

configure_file(version.hpp.in version.hpp)
target_include_directories(textgraphicslib PUBLIC "${PROJECT_BINARY_DIR}")

# build the executable test program last
target_link_libraries(textgridexe PRIVATE textgraphicslib ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} fontconfig)