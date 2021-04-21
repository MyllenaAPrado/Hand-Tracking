#include <opencv2/opencv.hpp>
#include <stdlib.h>   
#include <iostream>
#include "Segmentation.h"
#include "windows.h" 

int main()
{

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

	cv::UMat bgr_frame;
	Segmentation seg;

	//set the reference frame for do the segmentation
	cap >> bgr_frame;
	seg.setReferenceFrame(bgr_frame);

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		cv::imshow("capture", bgr_frame);

		//identify the pixels who change between the current frame and the reference frame
		seg.identifyMovingHand(bgr_frame);
		cv::imshow("segmentation", seg.getHandSegmentation());

		//identify the bigger contour in the segmentation image
		cv::imshow("contours", seg.identifyContours());


		char c = cv::waitKey(10);
		if (c == 27) break;
	}

	cap.release();
	return 0;
}

