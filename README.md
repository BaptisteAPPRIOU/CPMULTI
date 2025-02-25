# CPMULTI - Multi-Threaded Image Processing Application

This application provides real-time image processing capabilities using OpenCV with multi-threading support. It enables various filters to be applied to webcam feeds and benchmarks their performance with different thread counts.

## Features

- **Multiple Image Filters:**
  - Grayscale conversion
  - Gaussian blur
  - Median filter
  - Denoising (Non-local means)
  - Canny edge detection

- **Multi-threading Support:**
  - Configurable number of threads (1-10)
  - Performance analysis by filter type
  - Side-by-side comparison of filter execution speeds

- **Performance Visualization:**
  - Real-time performance graphs
  - Filter comparison metrics
  - Thread optimization analysis

- **User Interface:**
  - Interactive keyboard controls
  - Real-time filter application
  - Snapshot capability

## Requirements

- C++11 or higher
- OpenCV 4.x
- CMake 3.10 or higher
- A compatible webcam

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/BaptisteAPPRIOU/CPMULTI.git
   cd CPMULTI
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Configure and build:
   ```
   cmake ..
   cmake --build .
   ```

4. Make sure the `resources` directory exists in the project:
   ```
   mkdir -p ../resources
   ```

## Usage

Run the application:
```
./CPMULTI
```

### Keyboard Controls

| Key | Action |
|-----|--------|
| `g` | Apply Grayscale filter |
| `i` | Apply Gaussian filter |
| `o` | Apply Median filter |
| `p` | Apply Denoising filter |
| `c` | Apply Canny edge detection |
| `t` | Run performance tests for all filters |
| `v` | Visualize all filter performance metrics |
| `1` | View Grayscale filter performance only |
| `2` | View Gaussian filter performance only |
| `3` | View Median filter performance only |
| `4` | View Denoising filter performance only |
| `5` | View Canny filter performance only |
| `q` | Quit the application |

## Performance Analysis

The application includes a benchmarking tool that tests each filter with varying thread counts (1-10). Key findings:

- **Denoising filter** benefits significantly from multi-threading, with optimal performance at 4 threads
- **Lightweight filters** (Grayscale, Gaussian, Median, Canny) perform best with a single thread
- Adding more threads to lightweight operations generally degrades performance due to thread management overhead

### Performance Results

The visualization component displays execution times to help identify the optimal thread count for each filter type. For most operations:

- **CPU-intensive filters** (like Denoising): Benefit from 2-4 threads
- **Simple filters**: Perform best with a single thread

## Project Structure

```
CPMULTI/
├── Headers/                # Header files
│   ├── CannyFilter.hpp
│   ├── DenoisingFilter.hpp
│   ├── FaceDetection.hpp
│   ├── GaussianFilter.hpp
│   ├── GreyScaleFilter.hpp
│   ├── KeyHandler.hpp
│   ├── MedianFilter.hpp
│   ├── MultiThreadImageProcessor.hpp
│   ├── PerformanceVisualization.hpp
│   └── WebcamOperations.hpp
├── Sources/                # Implementation files
│   ├── CannyFilter.cpp
│   ├── DenoisingFilter.cpp
│   ├── FaceDetection.cpp
│   ├── GaussianFilter.cpp
│   ├── GreyScaleFilter.cpp
│   ├── KeyHandler.cpp
│   ├── MedianFilter.cpp
│   ├── MultiThreadImageProcessor.cpp
│   ├── PerformanceVisualization.cpp
│   └── WebcamOperations.cpp
├── resources/             # Resource files and saved images
├── main.cpp               # Application entry point
├── CMakeLists.txt         # CMake configuration
└── README.md              # This file
```

## Credits
Oussema Fatnassi, Baptiste Appriou and Ali Abakar Issa