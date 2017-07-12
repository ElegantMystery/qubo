#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/bgsegm.hpp"

//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

Mat im;
Mat gray;
Mat fgMaskMOG; //fg mask fg mask generated by MOG method
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
int keyboard; //input from keyboard


void processVideo(char* videoPath);
int main(int argc, char*argv[]){
    
    if(argc != 2){
        cout << "wrong number of arguments dummy, call \"a.out test_video\"" << endl;
        return 0;
    }
    
    pMOG = bgsegm::createBackgroundSubtractorMOG(1000,5,.7,0);
    
    processVideo(argv[1]);
    return 0;
}

void processVideo(char* videoPath){    
    // Read image
    VideoCapture cap;
    cap.open(videoPath);
    
    if(!cap.isOpened()){
        //error in opening the video input
        cerr << "Unable to open video file: " << videoPath << endl;
        exit(EXIT_FAILURE);
    }
   
 
    Mat im_with_keypoints;
    std::vector<KeyPoint> keypoints;
  
    while((char)keyboard != 'q' && (char)keyboard != 27 ){
         //read the current frame
        if(!cap.read(im)) {
            cerr << "Unable to read next frame." << endl;
            cerr << "Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        Mat canny_output, gauss, mask, invert;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        pMOG->apply(im, fgMaskMOG);
        GaussianBlur(fgMaskMOG, gauss, Size(15, 15), 2, 2 );
        
        /// Create Window
        imshow("source", im );
        imshow("GaussianBlur", gauss);

        // Define the structuring elements to be used in eroding and dilating the image 
        Mat se1 = getStructuringElement(MORPH_RECT, Size(6, 6));
        Mat se2 = getStructuringElement(MORPH_RECT, Size(3, 3));

        // Perform dialting and eroding helps to elminate background noise 
        morphologyEx(gauss, mask, MORPH_CLOSE, se1);
        morphologyEx(gauss, mask, MORPH_OPEN, se2);

        //inverts the colors 
        bitwise_not(mask, invert, noArray());

        int thresh = 84; //adjust this!!
        int max_thres = 255; //might want to play with this?
        
        /// Detect edges using canny
        Canny(invert, canny_output, thresh, thresh*2, 3 );
        
        namedWindow( "Canny", CV_WINDOW_AUTOSIZE );
        imshow( "Canny", canny_output );
        /// Find contours
        
        findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        
        /// Draw contours
        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
            {               
                Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }
        
        /// Show in a window
        namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
        imshow( "Contours", drawing );
        
        keyboard = waitKey(30);
    }
    cap.release();
}
