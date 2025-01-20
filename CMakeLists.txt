
cmake_minimum_required(VERSION 3.10)
project(CPMULTI)
find_package(OpenCV REQUIRED)
add_executable(CPMULTI main.cpp)
target_link_libraries(CPMULTI ${OpenCV_LIBS})