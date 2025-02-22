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

Mat MultiThreadImageProcessor::applyGreyscaleFilter(const cv::Mat& inputImage){
  if(inputImage.empty()){
    cout << "Empty image provided to the MultiThreadImageProcessor" << endl;
    return Mat();
  }

  int segmentHeight = inputImage.rows / numThreads;
  vector<thread> threads;
  vector<Mat> segments(numThreads);

  for(int i = 0; i < numThreads; i++){
    int startRow = i * segmentHeight;
    int endRow = (i == numThreads - 1) ? inputImage.rows : (i + 1) * segmentHeight;
    segments[i] = Mat(endRow - startRow, inputImage.cols, CV_8UC1);
    threads.emplace_back(&MultiThreadImageProcessor::processSegment, this, ref(inputImage), ref(segments[i]), startRow, endRow);
  }

  for(auto& thread : threads){
    thread.join();
  }

  Mat finalGreyscaleImage;
  vconcat(segments, finalGreyscaleImage);
  return finalGreyscaleImage;
}