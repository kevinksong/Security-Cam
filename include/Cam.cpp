#include "Cam.hpp"
using namespace std;

VideoCapture cap(0);

Camera::Camera(){
    //Check if camera is opened
    if (cap.isOpened()==false){ //If not opened, end program
        std::cout<< "Video camera cannot be opened."<<std::endl;
    }
    std::cout<< "Program starting..."<<std::endl;
    width=cap.get(CAP_PROP_FRAME_WIDTH); //Get width of the frames in the video
    height=cap.get(CAP_PROP_FRAME_HEIGHT); //Get height of the frames in the video
    fps=cap.get(CAP_PROP_FPS); //Get frames per second of the video 
    //Print values out
    std::cout<< "Width: "<<width<< " Height: "<<height<< " FPS: "<< fps << std::endl;
    //Read 3 frames: the current, previous, and next
    cap>>prev_frame;
    cvtColor(prev_frame,prev_frame,COLOR_RGB2GRAY); //Convert prev_frame to grayscale
    cap>>current_frame;
    cvtColor(current_frame,current_frame,COLOR_RGB2GRAY); //Convert prev_frame to grayscale
    cap>>next_frame;
    cvtColor(next_frame,next_frame,COLOR_RGB2GRAY); //Convert prev_frame to grayscale

    //Number of changes in the resulting image
    number_of_changes=0;

    //Motion detection

    x_start=10;
    x_end=width-10;
    y_start=10;
    y_end=height-10;

    //Max deviation allowed in image, higher value = more motion allowed

    max_deviation=20;

    //Erode kernel
    ero_kernel=getStructuringElement(MORPH_RECT, Size(2,2));

    //Intialise video writer

    codec=VideoWriter::fourcc('H','2','6','4'); //codec H.264
    

}

Camera::~Camera(){
    cap.release(); //release camera
    writer.release(); //release VideoWriter
}

Mat Camera::capture_video(){
    //Read new frame
    cap>>frame;
    //Copy of frame
    result=frame;
    //Display frame in grayscale
    cvtColor(frame,frame,COLOR_RGB2GRAY);
    //imshow using copy of frame
    cv::namedWindow("Motion Detector", WINDOW_AUTOSIZE);
    cv::imshow("Motion Detector",result);
    //Put images in correct order
    prev_frame=current_frame;
    current_frame=next_frame;
    next_frame=frame;
    is_motion=diff_img(prev_frame,current_frame,next_frame);
    return result;
}

//Calculate differences between frames
Mat Camera::diff_img(Mat prev, Mat current, Mat next){
    absdiff(prev,next,diff1);
    absdiff(next,current,diff2);
    bitwise_and(diff1,diff2,is_motion);
    threshold(is_motion,is_motion,35,255,THRESH_BINARY);
    erode(is_motion,is_motion,ero_kernel);
    return is_motion;
}

int Camera::detect_motion(){
    //Calculate standard deviation
    meanStdDev(is_motion,mean,std_dev);
    
    if (std_dev[0]<max_deviation){
        number_of_changes=0;
        x_min=is_motion.cols, x_max=0;
        y_min=is_motion.cols, y_max=0;
        for (int j=y_start;j<y_end;j+=2){
            for (int i=x_start;i<x_end;i+=2){
                if( static_cast<int>(is_motion.at<uchar>(j,i)) == 255){
                    number_of_changes++;
                    if (x_min>i){
                        x_min=i;
                    }
                    if (x_max<i){
                        x_max=i;
                    }
                    if(y_min<j){
                        y_min=j;
                    }
                    if(y_max>j){
                        y_max=j;
                    }
                }
            }
        }
        if (number_of_changes!=0){
                        if (x_min-10>0){
                            x_min-=10;
                        }
                        if (y_min-10>0){
                            y_min-=10;
                        }
                        if (x_max+10<frame.cols-1){
                            x_max+=10;
                        }
                        if (y_max+10>frame.rows-1){
                            y_max+=10;
                        }
                        Point x(x_min,y_min);
                        Point y(x_max,y_max);
                        Rect rect(x,y);
                        rectangle(result,rect,Scalar(0,255,255),1,4);
                        imshow("Motion Indicator", result);
                }
                return number_of_changes;
    }
    return 0;
}

void Camera::open_video(char detectionfile [80]){
    string filename= detectionfile;
    writer.open(filename,codec,fps,frame.size(),true);
    if (!writer.isOpened()){
        std::cerr<< "Could not open video file "<<std::endl;
    }
}

void Camera::save_video(Mat oneFrame){
    //Creates window used as placeholder
    namedWindow("SAVING", WINDOW_AUTOSIZE);
    imshow("SAVING", oneFrame);
    //Suspends execution of calling thread for n microseconds
    usleep(100);

    writer << oneFrame;
}

void Camera::close_video(){
    writer.release(); //Release VideoWriter();
}