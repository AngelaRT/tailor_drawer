#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>

cv::VideoCapture TheVideoCapturer;
cv::Mat bgrMap;

int main(int argc, char *argv[]) {
  char key = 0;

  int numSnapshot = 0;
  std::string snapshotFilename = "0";

  std::cout << "Press 's' to take snapshots" << std::endl;
  std::cout << "Press 'Esc' to exit" << std::endl;

  TheVideoCapturer.open(0);

  if (!TheVideoCapturer.isOpened()) {
    std::cerr<<"Could not open video"<<std::endl;
    return -1;
  }

  while (key!=27 && TheVideoCapturer.grab()) {
    TheVideoCapturer.retrieve(bgrMap);

    cv::imshow("BGR image", bgrMap);

    if (key == 115) {
      cv::imwrite(snapshotFilename + ".png", bgrMap);
      numSnapshot++;
      snapshotFilename = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
        }

  key=cv::waitKey(20);
  }
}