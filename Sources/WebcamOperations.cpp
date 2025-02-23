#include "Headers/WebcamOperations.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include "Headers/GreyScaleFilter.hpp"

// Constructor
WebcamOperations::WebcamOperations() : imageProcessor(4) {
    cout << "WebCamOperations initialized." << endl;
}

// Destructor
WebcamOperations::~WebcamOperations() {
    closeWebcam();
}

// Open Webcam
void WebcamOperations::openWebcam() {
    if (!cap.open(0)) {
        cerr << "Error: Unable to access the webcam." << endl;
        return;
    }
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'q' to quit." << endl;

    namedWindow(windowName, cv::WINDOW_NORMAL);
    resizeWindow(windowName, 400, 300);

    while(true){
        cap >> frame;
        if(frame.empty()){
            cerr << "Error: No frame available from the webcam." << endl;
            break;
        }

        imshow(windowName, frame);

        char key = waitKey(10);
        if(key == 'q'){
            break;
        } else if (key == 't') {
            takeSnapShot(frame, "snapshot.jpg");
            Mat savedSnapshot = imread(resourcesPath + "/snapshot.jpg");
            if (savedSnapshot.empty()) {
                cerr << "Error: Unable to load the saved snapshot." << endl;
                return;
            }
        
            // Run multiple iterations to get more accurate timing
            const int iterations = 1000;
            
            // Test different thread counts
            for (int threads = 1; threads <= 8; threads++) {
                imageProcessor.setNumThreads(threads);
                double totalDuration = 0;
                
                for (int i = 0; i < iterations; i++) {
                    auto [_, duration] = imageProcessor.applyGreyscaleFilterTimed(savedSnapshot);
                    totalDuration += duration;
                }
                
                cout << "Average processing time with " << threads << " threads: " 
                     << (totalDuration / iterations) << " µs" << endl;
            }
        // For the 'g' key handler:
        } else if(key == 'g'){
            takeSnapShot(frame, "snapshot.jpg");
        
            Mat savedSnapshot = imread(resourcesPath + "/snapshot.jpg");
            if(savedSnapshot.empty()){
                cerr << "Error: Unable to load the saved snapshot." << endl;
                break;
            }
        
            auto [greyscaleFrame, duration] = imageProcessor.applyGreyscaleFilterTimed(savedSnapshot);
        
            if(!greyscaleFrame.empty()){
                namedWindow("Greyscale Feed", WINDOW_NORMAL);
                imshow("Greyscale Feed", greyscaleFrame);
                cout << "Greyscale processing time with " << imageProcessor.getNumThreads() 
                     << " threads: " << duration << " µs" << endl;
            }
        }
    }
    closeWebcam();
}

// Take a Snapshot
void WebcamOperations::takeSnapShot(const cv::Mat& inputFrame, const std::string& filename) {
    if (inputFrame.empty()) {
        cerr << "Error: No frame available to take a snapshot." << endl;
        return;
    }

    // Save the provided frame as the snapshot
    snapshot = inputFrame.clone();
    snapShotName = filename; // Set the filename for saving
    cout << "Snapshot taken and stored in memory." << endl;
    saveSnapShot();
}

// Save the Snapshot
void WebcamOperations::saveSnapShot() {
    if (snapshot.empty()) {
        cerr << "Error: No snapshot available to save." << endl;
        return;
    }

    // Ensure the resources path exists
    if (!filesystem::exists(resourcesPath)) {
        filesystem::create_directories(resourcesPath);
    }

    // Generate a unique filename
    string baseName = snapShotName.substr(0, snapShotName.find_last_of('.'));
    string extension = snapShotName.substr(snapShotName.find_last_of('.'));
    string fullPath = resourcesPath + "/" + snapShotName;
    int counter = 1;

    // Check for existing files and append a counter if needed
    while (filesystem::exists(fullPath)) {
        fullPath = resourcesPath + "/" + baseName + std::to_string(counter) + extension;
        counter++;
    }

    // Save the snapshot
    if (imwrite(fullPath, snapshot)) {
        cout << "Snapshot saved at " << fullPath << "." << endl;
    } else {
        cerr << "Error: Unable to save the snapshot." << endl;
    }
}

// Close the Webcam
void WebcamOperations::closeWebcam() {
    if (cap.isOpened()) {
        cap.release();
        destroyAllWindows();
        cout << "Webcam closed successfully." << endl;
    }
}

// Set Resources Path
void WebcamOperations::setResourcesPath(const string& path) {
    resourcesPath = path;
    cout << "Resources path set to: " << resourcesPath << endl;
}