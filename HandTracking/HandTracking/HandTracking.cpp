#include <opencv2/opencv.hpp>
#include <stdlib.h>   
#include <iostream>
#include "Segmentation.h"
#include "Fingers.h"
#include "windows.h" 

int main()
{

	cv::namedWindow("capture", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("segmentation", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("contours", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("fingers", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't op en capture." << std::endl;
		return -1;
	}

	cv::UMat bgr_frame;
	Segmentation seg;
	Fingers fingers;

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
		if (!seg.getcontours().empty()) {
			cv::imshow("fingers", fingers.getFingerPoints(bgr_frame, seg));
		}
		else {
			cv::imshow("fingers", bgr_frame);
		}
				
		char c = cv::waitKey(10);
		if (c == 27) break;
		//set a new reference frame for segmentation
		if (c == 49) {
			seg.setReferenceFrame(bgr_frame);
		}
	}

	cap.release();
	return 0;
}

