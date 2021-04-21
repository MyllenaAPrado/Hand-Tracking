#include <opencv2/opencv.hpp>
#include <stdlib.h>   
#include <iostream>
#include "Segmentation.h"
#include "windows.h" 

int main()
{
	int flag_roi = 0;
	cv::namedWindow("capture", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("segmentation", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("contours", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't op en capture." << std::endl;
		return -1;
	}
	int count = 0;
	cv::UMat bgr_frame, img_filter;
	Segmentation seg;
	cap >> bgr_frame;
	seg.setReferenceFrame(bgr_frame);

	for (;;)
	{
		cap >> bgr_frame;

		//cv::Rect rect = cv::Rect(80, 150, 150, 190);
		//cv::rectangle(bgr_frame, rect, cv::Scalar(255, 0, 0), 1, 8, 0);

		if (bgr_frame.empty()) break;


		cv::imshow("capture", bgr_frame);

		char c = cv::waitKey(10);
		if (c == 27) break;

		seg.identifyMovingHand(bgr_frame);

		cv::imshow("segmentation", seg.getHandSegmentation());

		cv::imshow("contours", seg.identifyContours());

	}

	cap.release();
	return 0;
}

