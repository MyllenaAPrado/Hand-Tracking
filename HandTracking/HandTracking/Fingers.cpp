#include "Fingers.h"
#include "Segmentation.h"
#include <math.h>

cv::UMat Fingers::getFingerPoints(cv::UMat frame, Segmentation seg) {
	int largestComp = seg.geIndexLargContour();
	std::vector<std::vector<cv::Point>> contours = seg.getcontours();
	std::vector<cv::Vec4i> convexityDefects(contours.size());
	std::vector<cv::Point> pointers(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	cv::Point point_reference(1000000,0);
	std::vector<int> hull_ints;
	int threshold_points = 50;
	frame.copyTo(this->frame);

	//get the points interception between convexHull and contours
	cv::convexHull(contours[largestComp], hull_ints, false);

	std::vector<std::vector<cv::Point>> hull(contours.size());
	cv::convexHull(contours[largestComp], hull[0]);
	if (hull_ints.size() > 3)
		cv::convexityDefects(cv::Mat(contours[largestComp]), hull_ints, convexityDefects);

	//get the rectangulo around the hand
	boundRect[0] = cv::boundingRect(contours[largestComp]);
	int yCenter = boundRect[0].y + (boundRect[0].height / 2);
	int xCenter = boundRect[0].x + (boundRect[0].width / 2);
	cv::Point pointCenter(xCenter, yCenter + 90);

	//check if the point has the desired angle 
	for (size_t i = 0; i < convexityDefects.size(); i++)
	{
		cv::Point p1 = contours[largestComp][convexityDefects[i][0]];
		cv::Point p2 = contours[largestComp][convexityDefects[i][1]];
		cv::Point p3 = contours[largestComp][convexityDefects[i][2]];

		//filter points above the y of rectangulo center
		if ((p3.y - p2.y) > (threshold_points) && (p3.y < (yCenter+ threshold_points)) && 
			(p2.y < (yCenter + threshold_points)) && (p1.y < (yCenter + threshold_points))) {

			//calcule angle
			double beta = atan2((double(p3.x) - double(p1.x)), (double(p3.y) - double(p1.y)));
			double alpha = atan2((double(p2.x) - double(p3.x)), (double(p2.y) - double(p3.y)));
			double theta = alpha + beta;
			theta = (theta * 180) / this->PI;
			if (theta <= 90) {

				pointers.push_back(p1);

				if (p2.x < point_reference.x) {
					point_reference = p2;
				}
			}
		}

	}

	pointers.push_back(point_reference);

	//plot points and line of hand model in image
	if (!pointers.empty()) {
		for (int y = 1; y < int(pointers.size()); y++) {
			if (pointers[y].x != 0 && pointers[y].y != 0) {
				cv::circle(this->frame, pointers[y], 10, this->color_green);
				cv::line(this->frame, pointCenter, pointers[y], this->color_green, 2);
				cv::circle(this->frame, pointers[1], 10, this->color_red);
			}
		}

	}

	//flip the output image
	cv::flip(this->frame, this->frame, 1);
	return this->frame;
}

//not used
std::vector<cv::Point> Fingers::averagePointers(std::vector<cv::Point> pointers, std::vector<cv::Point> pointers_between_hand) {
	double thresholdDistance = 20;
	cv::Point referencePoint, actualPoint;
	std::vector<cv::Point> averagePointers(pointers.size());
	
	//check the distance between points and make the average
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
			cv::Point point((averageX / quantityNumber), referencePoint.y);
			averagePointers.push_back(point);
			referencePoint = actualPoint;
			averageX = actualPoint.x;
			quantityNumber = 1;
		}
	}
	return averagePointers;
}

//not used
double Fingers::PointDistance(cv::Point a, cv::Point b) {

	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));

}