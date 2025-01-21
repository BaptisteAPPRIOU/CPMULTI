#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h> // for windows

using namespace cv;

int main(int argc, char** argv) {
    Mat image;
    
    // Load image from the same directory
    image = imread("test.jpg");
    
    if (image.empty()) {
        std::cout << "Error: Could not load image" << std::endl;
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return -1;
    }
    
    std::cout << "Image loaded successfully" << std::endl;
    std::cout << "Image size: " << image.size() << std::endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    
    std::cout << "Press any key to exit..." << std::endl;
    waitKey(0);
    destroyAllWindows();
    
    return 0;
}