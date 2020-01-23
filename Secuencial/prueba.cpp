#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv) {
 // We'll start by loading an image from the drive
 Mat image = imread("/Users/Karla/Documents/Computo Paralelo/Proyecto Secuencial/images/Lenna.png");
 // We check that our image has been correctly loaded
 if(image.empty()) {
 std::cout << "Error: the image has been incorrectly loaded." << std::endl;
 return 0;
 }
 // Then we create a window to display our image
 namedWindow("My first OpenCV window");
 // Finally, we display our image and ask the program to wait for a key to be pressed
 imshow("My first OpenCV window", image);
 waitKey(0);
 return 0;
}