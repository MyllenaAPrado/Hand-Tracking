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


public:
	std::vector<std::vector<cv::Point>> contours;
	int largestComp = 0;

	inline cv::UMat getHandSegmentation() {
		return segment_hand;
	}
	inline void setReferenceFrame(cv::UMat frame) {
		cv::GaussianBlur(frame, this->reference_frame, cv::Size(3, 3), 0);
		cv::cvtColor(this->reference_frame, this->reference_frame, cv::COLOR_BGR2GRAY);
	}
	void identifyMovingHand(cv::UMat frame);
	cv::UMat identifyContours();

};

#endif