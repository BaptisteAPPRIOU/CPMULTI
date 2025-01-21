cmake_minimum_required(VERSION 3.10)

# Project Name
project(CPMULTI)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include OpenCV
find_package(OpenCV REQUIRED)

# Detect platform for OS-specific configurations 
if(APPLE)
    message(STATUS "Compiling for macOS")
    # macOS-specific flags (if any)
elseif(WIN32)
    message(STATUS "Compiling for Windows")
    # Windows-specific flags (if any)
endif()

# Source and Header Files
# Collect all .cpp and .hpp files from their respective directories and subdirectories
file(GLOB_RECURSE SOURCES "Sources/*.cpp")
file(GLOB_RECURSE HEADERS "Headers/*.hpp")

# Add executable
add_executable(CPMULTI main.cpp ${SOURCES} ${HEADERS})

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS} Headers)

# Link Libraries
target_link_libraries(CPMULTI ${OpenCV_LIBS})

# Copy resource files to the output directory
file(GLOB RESOURCES "resources/*")
foreach(RESOURCE ${RESOURCES})
    configure_file(${RESOURCE} ${CMAKE_BINARY_DIR}/resources COPYONLY)
endforeach()

# Output messages
message(STATUS "Sources: ${SOURCES}")
message(STATUS "Headers: ${HEADERS}")
message(STATUS "Resources: ${RESOURCES}")
