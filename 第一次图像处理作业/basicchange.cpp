#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <iostream>
#include <chrono>
#include "basicchange.h"
using namespace cv;
cv::Mat basicchange(bool sym, cv::Mat image, int angle, float size)
{
	resize(image, image, Size(int(image.cols * size), int(image.rows * size)), INTER_CUBIC);
	Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
	Mat rotation_matix = getRotationMatrix2D(center, angle, 1.0);
	// we will save the resulting image in rotated_image matrix
	Mat rotated_image;
	// rotate the image using warpAffine
	warpAffine(image, rotated_image, rotation_matix, image.size());
	if (sym)
		flip(rotated_image, rotated_image, 1);
	//return rotated_image;
	//rotated_image = resize(rotated_image, rotated_image.cols * 0.6, int(rows * 1.2)))
	return rotated_image;

}
