#include "Headers/MultiThreadImageProcessor.hpp"


MultiThreadImageProcessor::MultiThreadImageProcessor(int numThreads) {
    this->numThreads = numThreads;
}

MultiThreadImageProcessor::~MultiThreadImageProcessor() {
}

void MultiThreadImageProcessor::processSegment(const Mat& input, Mat& output, int startRow, int endRow) {
    Mat segment = input(Range(startRow , endRow), Range::all());
    output = greyScaleFilter.applyFilter(segment);
}

Mat MultiThreadImageProcessor::applyGreyscaleFilter(const cv::Mat& inputImage) {
  auto [result, _] = applyGreyscaleFilterTimed(inputImage);
  return result;
}

#include "Headers/MultiThreadImageProcessor.hpp"

pair<Mat, double> MultiThreadImageProcessor::applyGreyscaleFilterTimed(const Mat& inputImage) {
  if(inputImage.empty()) {
      cout << "Empty image provided to the MultiThreadImageProcessor" << endl;
      return {Mat(), 0};
  }

  // Create output image once
  Mat finalGreyscaleImage(inputImage.rows, inputImage.cols, CV_8UC1);
  
  auto startTime = high_resolution_clock::now();

  if (numThreads <= 1) {
      // Single-threaded case
      finalGreyscaleImage = greyScaleFilter.applyFilter(inputImage);
  } else {
      // Multi-threaded case
      vector<thread> threads;
      int segmentHeight = inputImage.rows / numThreads;
      
      for(int i = 0; i < numThreads; i++) {
          int startRow = i * segmentHeight;
          int endRow = (i == numThreads - 1) ? inputImage.rows : (i + 1) * segmentHeight;
          
          threads.emplace_back([&, startRow, endRow]() {
              Mat inputSegment = inputImage(Range(startRow, endRow), Range::all());
              Mat outputSegment = finalGreyscaleImage(Range(startRow, endRow), Range::all());
              greyScaleFilter.applyFilter(inputSegment).copyTo(outputSegment);
          });
      }

      for(auto& thread : threads) {
          thread.join();
      }
  }

  auto stopTime = high_resolution_clock::now();
  // Convert to microseconds instead of milliseconds
  auto duration = duration_cast<microseconds>(stopTime - startTime);

  return {finalGreyscaleImage, duration.count()};
}

void MultiThreadImageProcessor::setNumThreads(int numThreads) {
  this->numThreads = numThreads;
}

int MultiThreadImageProcessor::getNumThreads() const {
  return numThreads;
}