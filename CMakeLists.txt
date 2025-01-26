cmake_minimum_required(VERSION 3.16)
project(CPMULTI VERSION 0.1 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV first
# Find OpenCV first
set(OpenCV_DIR "C:/msys64/mingw64/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

# Define directories for sources, headers, and resources
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/Sources")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/Headers")
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

# Debug output to verify paths
message(STATUS "Source directory: ${SOURCE_DIR}")
message(STATUS "Header directory: ${HEADER_DIR}")
message(STATUS "Resource directory: ${RESOURCE_DIR}")

# Automatically collect all .cpp and .hpp files
file(GLOB_RECURSE PROJECT_SOURCES 
    "${SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)
file(GLOB_RECURSE PROJECT_HEADERS "${HEADER_DIR}/*.hpp")

# Output found files for debugging
message(STATUS "Found source files: ${PROJECT_SOURCES}")
message(STATUS "Found header files: ${PROJECT_HEADERS}")

# Add executable target
add_executable(CPMULTI ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Include header directories and link libraries (only once)
target_include_directories(CPMULTI PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${HEADER_DIR} 
    ${OpenCV_INCLUDE_DIRS}
)

# Link OpenCV libraries
target_link_libraries(CPMULTI PRIVATE ${OpenCV_LIBS})

# Create resources directory in build
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resources)

# Copy resources to the build directory
file(GLOB RESOURCES "${RESOURCE_DIR}/*")
foreach(RESOURCE ${RESOURCES})
    configure_file(${RESOURCE} ${CMAKE_BINARY_DIR}/resources COPYONLY)
endforeach()

# Output final diagnostic messages
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "Resources to copy: ${RESOURCES}")