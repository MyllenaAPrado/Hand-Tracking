#ifndef Fingers_H_INCLUDED
#define Fingers_INCLUDED
#include <opencv2/opencv.hpp>
#include <vector>
#include "Segmentation.h"

class Fingers
{
private:
	std::vector<cv::Point> points_hand;
	cv::Scalar color_blue = cv::Scalar(255, 0, 0);
	cv::Scalar color_green = cv::Scalar(0, 255, 0);
	cv::Scalar color_red = cv::Scalar(0, 0, 255);
	cv::UMat frame;
	const double PI = 3.14159265;

public:

	cv::UMat getFingerPoints(cv::UMat frame, Segmentation seg);
	std::vector<cv::Point> averagePointers(std::vector<cv::Point> pointers);
	double PointDistance(cv::Point a, cv::Point b);
	
};

#endif