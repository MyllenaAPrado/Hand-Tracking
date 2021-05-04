#ifndef Segmentation_H_INCLUDED
#define Segmentation_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core/types.hpp>

class Segmentation
{
private:
	cv::UMat segment_hand;
	cv::UMat reference_frame;
	cv::UMat current_frame;
	cv::Scalar color_blue = cv::Scalar(255, 0, 0);
	cv::Scalar color_green = cv::Scalar(0, 255, 0);
	cv::Scalar color_red = cv::Scalar(0, 0, 255);
	cv::Scalar color_black = cv::Scalar(0, 0, 0);
	std::vector<std::vector<cv::Point>> contours;
	int largestComp = 0;


public:
	inline cv::UMat getHandSegmentation() {
		return this->segment_hand;
	}
	inline std::vector<std::vector<cv::Point>> getcontours() {
		return this->contours;
	}
	inline int geIndexLargContour() {
		return this->largestComp;
	}
	void setReferenceFrame(cv::UMat frame);
	void identifyMovingHand(cv::UMat frame);
	cv::UMat identifyContours();

};

#endif