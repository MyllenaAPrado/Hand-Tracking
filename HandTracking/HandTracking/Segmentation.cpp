#include "Segmentation.h"

void Segmentation::identifyMovingHand(cv::UMat frame) {

	cv::UMat processing_frame;

	//gaussian filter for improve the image
	cv::GaussianBlur(frame, processing_frame, cv::Size(3, 3), 0);
	cv::cvtColor(processing_frame, this->current_frame, cv::COLOR_BGR2GRAY);

	//calcule the diference of pixels
	cv::absdiff(this->reference_frame, this->current_frame, this->segment_hand);
	cv::threshold(this->segment_hand, this->segment_hand, 35, 255, cv::THRESH_BINARY);
	cv::dilate(this->segment_hand, this->segment_hand, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));

}

cv::UMat Segmentation::identifyContours() {

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	int idx = 0, largestComp = 0;
	double maxArea = 0;
	
	//find all contours in image
	cv::findContours(this->segment_hand, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::UMat imgContours(this->segment_hand.size(), CV_8UC3, cv::Scalar(0.0, 0.0, 0.0));

	//identify the bigger contour
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		const std::vector<cv::Point>& c = contours[idx];
		double area = fabs(cv::contourArea(cv::Mat(c)));
		if (area > maxArea)
		{
			maxArea = area;
			largestComp = idx;
		}
	}

	//draw contour in image
	drawContours(imgContours, contours, largestComp, cv::Scalar(0.0, 200.0, 0.0), cv::FILLED, cv::LINE_8, hierarchy);

	return imgContours;
}