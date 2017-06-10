#ifndef FIND_BUOY_ACTION_H
#define FIND_BUOY_ACTION_H


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
#include <tuple>        // std::tuple, std::get, std::tie, std::ignore
#include <vector>

#include "vision_node.h"


class FindBuoyAction{
    public:
    FindBuoyAction(actionlib::SimpleActionServer<ram_msgs::VisionExampleAction> *as);
    ~FindBuoyAction();

    cv::Mat backgroundSubtract(cv::Mat cframe);
    std::vector<cv::KeyPoint> detectBuoy(cv::Mat cframe);
	void updateAction(cv::Mat cframe);
	bool updateHistory(cv::Mat cframe, std::vector<cv::KeyPoint> keypoints, cv::Point2f center);
    
    protected:


	actionlib::SimpleActionServer<ram_msgs::VisionExampleAction> *m_as;

	cv::Ptr<cv::SimpleBlobDetector> m_detector;
	
	cv::Ptr<cv::BackgroundSubtractor> m_pMOG;//MOG Background subtractor
    
    std::vector<std::tuple<cv::Point2f, cv::Vec3b, int>> m_history;

	ram_msgs::VisionExampleFeedback m_feedback;

	
};

#endif
