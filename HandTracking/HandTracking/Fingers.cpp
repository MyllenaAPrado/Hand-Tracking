#include "Fingers.h"
#include "Segmentation.h"
#include <math.h>

cv::UMat Fingers::getFingerPoints(cv::UMat frame, Segmentation seg) {
	int largestComp = seg.geIndexLargContour();
	std::vector<std::vector<cv::Point>> contours = seg.getcontours();
	this->frame = frame;
	std::vector<cv::Vec4i> convexityDefects(contours.size());
	std::vector<cv::Point> pointers(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	std::vector<int> hull_ints;
	int thresholdRect = 20;

	//get the points interception between convexHull and contours
	cv::convexHull(contours[largestComp], hull_ints, false);
	if (hull_ints.size() > 3)
		cv::convexityDefects(cv::Mat(contours[largestComp]), hull_ints, convexityDefects);

	//get the rectangulo around the hand
	boundRect[0] = cv::boundingRect(contours[largestComp]);
	int yCenter = (boundRect[0].tl().y + boundRect[0].br().y) / 2;
	int xCenter = (boundRect[0].tl().x + boundRect[0].br().x) / 2;
	cv::Point pointCenter(xCenter, yCenter+ thresholdRect);
	cv::rectangle(this->frame, boundRect[0].tl(), boundRect[0].br(), this->color_blue, 2);

	//check if the point has the desired angle 
	for (size_t i = 0; i < convexityDefects.size(); i++)
	{
		cv::Point p1 = contours[largestComp][convexityDefects[i][0]];
		cv::Point p2 = contours[largestComp][convexityDefects[i][1]];
		cv::Point p3 = contours[largestComp][convexityDefects[i][2]];
		cv::line(this->frame, p1, p3, this->color_blue, 2);
		cv::line(this->frame, p3, p2, this->color_green, 2);

		//filter points above the y of rectangulo center
		if (p2.y < (yCenter + thresholdRect)) {
			//calcule angle
			double teta = atan2((double(p3.x) - double(p2.x)), (double(p2.y) - double(p3.y)));
			double beta = atan2((double(p2.x) - double(p1.x)), (double(p2.y) - double(p1.y)));
			double alpha = teta + beta;
			alpha = (alpha * 180) / this->PI;

			if (alpha <= 60) {
				cv::circle(this->frame, p2, 10, this->color_red);
				pointers.push_back(p2);
			}
		}

	}

	//filter the nearby points with average and get the final points of hand
	std::vector<cv::Point> pointsFinger = averagePointers(pointers);	
	for (int y = 0; y < int(pointsFinger.size()); y++) {
		if (pointsFinger[y].x != 0 && pointsFinger[y].y != 0) {
			cv::circle(this->frame, pointsFinger[y], 20, this->color_green);
			cv::line(this->frame, pointCenter, pointsFinger[y], this->color_green, 2);
		}
	}
	this->points_hand = pointsFinger;
	return this->frame;
}

std::vector<cv::Point> Fingers::averagePointers(std::vector<cv::Point> pointers) {

	double thresholdDistance = 50;
	cv::Point referencePoint, actualPoint;
	std::vector<cv::Point> averagePointers(pointers.size());
	
	//check the distance between points and make the average
	if (!pointers.empty()) {
		referencePoint = pointers[0];
		int averageX = pointers[0].x;
		int quantityNumber = 1;

		for (int i = 1; i < int(pointers.size()); i++) {
			actualPoint = pointers[i];
			if (PointDistance(referencePoint, actualPoint) < thresholdDistance) {
				averageX += actualPoint.x;
				quantityNumber++;
			}
			else {
				cv::Point point((averageX/ quantityNumber), referencePoint.y);
				averagePointers.push_back(point);
				referencePoint = actualPoint;
				averageX = actualPoint.x;
				quantityNumber = 1;
			}
		}
	}
	return averagePointers;
}

double Fingers::PointDistance(cv::Point a, cv::Point b) {

	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));

}