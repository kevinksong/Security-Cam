#ifndef __IOSTREAM_INCLUDED__
#define __IOSTREAM_INCLUDED__
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <unistd.h>

using namespace cv;
using namespace std;

class Camera{
    public:
    Camera(); //Constructor
    ~Camera(); //Destructor 
    Mat capture_video();
    Mat diff_img(Mat,Mat,Mat);
    void open_video(char[80]); 
    void save_video(Mat);
    void close_video();
    int detect_motion(void); 
    private:
    VideoWriter writer;

    double fps;
    double width;
    double height;
    Mat frame;
    Mat prev_frame;
    Mat current_frame;
    Mat next_frame;
    Mat diff1;
    Mat diff2;
    Mat is_motion;
    Mat result;
    int x_start, x_end;
    int y_start, y_end;
    int max_deviation;
    Scalar mean, std_dev;
    int x_min, y_min;
    int x_max, y_max;
    int codec;
    int number_of_changes;
    Mat ero_kernel;
};
#endif	// __IOSTREAM_INCLUDED__
