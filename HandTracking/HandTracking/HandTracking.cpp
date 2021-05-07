#include <opencv2/opencv.hpp>
#include <stdlib.h>   
#include <iostream>
#include "Segmentation.h"
#include "Fingers.h"
#include "windows.h" 

int main()
{

	cv::namedWindow("Hand tracking", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return -1;
	}

	cv::UMat bgr_frame;
	Segmentation segmentation;
	Fingers fingers;

	//set the reference frame for do the segmentation
	cap >> bgr_frame;
	segmentation.setReferenceFrame(bgr_frame);

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		//identify the pixels who change between the current frame and the reference frame
		segmentation.identifyMovingHand(bgr_frame);
		segmentation.getHandSegmentation();
		segmentation.identifyContours();

		if (!segmentation.getcontours().empty()) {
			cv::imshow("Hand tracking", fingers.getFingerPoints(bgr_frame, segmentation));
		}
		else {
			cv::flip(bgr_frame, bgr_frame, 1);
			cv::imshow("Hand tracking", bgr_frame);
		}
				
		char c = cv::waitKey(10);

		//set a new reference frame for segmentation
		if (c == 49) {
			segmentation.setReferenceFrame(bgr_frame);
		}

		if (c == 27) break;

	}

	cap.release();
	return 0;
}

