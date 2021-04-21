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

public:

	inline cv::UMat getHandSegmentation() {
		return segment_hand;
	}
	inline void setReferenceFrame(cv::UMat frame) {
		cv::cvtColor(frame, this->reference_frame, cv::COLOR_BGR2GRAY);
	}
	void identifyMovingHand(cv::UMat frame);
	cv::UMat identifyContours();

};

#endif