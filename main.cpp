#include <iostream>
#include <opencv2/opencv.hpp>
# include <windows.h>

using namespace cv;

int main(int argc, char** argv) {
    Mat image;
    std::cout << "Loading image..." << std::endl;
    
    // load image
    image = imread("test_image.jpg");
    
    if (image.empty()) {
        std::cout << "Error: Could not load image" << std::endl;
        std::cout << "Searching for image in directory..." << std::endl;
        system("dir test_image.jpg");
        system("pause");
        return -1;
    }
    
    std::cout << "Original image size: " << image.size() << std::endl;
    
    // Image resizer
    Mat resized_image;
    resize(image, resized_image, Size(800, 800), 0, 0, INTER_LINEAR);
    
    std::cout << "Resized image size: " << resized_image.size() << std::endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", resized_image);
    
    std::cout << "Press any key to exit..." << std::endl;
    waitKey(0);
    
    return 0;
}