cmake_minimum_required(VERSION 3.0.0)
project(opencvtest VERSION 0.1.0)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV path - make sure this points to your actual OpenCV installation
set(OpenCV_DIR "C:/opencv-mingw/x64/mingw/lib")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${OpenCV_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(opencvtest main.cpp)
target_link_libraries(opencvtest ${OpenCV_LIBS})

# Copy OpenCV DLLs to build directory
file(GLOB OpenCV_DLLS "${OpenCV_DIR}/../bin/*.dll")
file(COPY ${OpenCV_DLLS} DESTINATION ${CMAKE_BINARY_DIR})

# Debug messages
message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
message(STATUS "DLLs to copy: ${OpenCV_DLLS}")