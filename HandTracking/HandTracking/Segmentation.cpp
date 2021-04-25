#include "Segmentation.h"

void Segmentation::setReferenceFrame(cv::UMat frame) {
	cv::GaussianBlur(frame, this->reference_frame, cv::Size(3, 3), 0);
	cv::cvtColor(this->reference_frame, this->reference_frame, cv::COLOR_BGR2GRAY);
}

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
	
	double maxArea = 0;
	
	//find all contours in image
	cv::findContours(this->segment_hand, this->contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::UMat imgContours(this->segment_hand.size(), CV_8UC3, this->color_red);

	//identify the bigger contour
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
	{
		if (this->contours.empty()) {
			return imgContours;
		}
		const std::vector<cv::Point>& c = this->contours[idx];
		double area = fabs(cv::contourArea(cv::Mat(c)));
		if (area > maxArea)
		{
			maxArea = area;
			this->largestComp = idx;
		}
	}

	//draw contour in image
	drawContours(imgContours, this->contours, this->largestComp, this->color_green, cv::FILLED, cv::LINE_8, hierarchy);

	//draw the retangule of hand in the image
	std::vector<std::vector<cv::Point>> hull(this->contours.size());
	cv::convexHull(this->contours[this->largestComp], hull[0]);
	drawContours(imgContours, hull, (int)0, cv::Scalar(250.0, 0.0, 0.0));
	return imgContours;
}