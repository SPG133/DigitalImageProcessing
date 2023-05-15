#include <iostream>
using namespace std;
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include<chrono>
#include<windows.h>
#include"open.c"
#include"Roberts.h"
#include"OTSU.h"
#include"RegionGrowing.h"
#include"FaceDetect.h"
using namespace cv;
#define WINDOW_NAME "简易图像处理"
int main()
{
	//char filename[50]="C:\\Users\\S\\Desktop\\cat.bmp";
	cv::Mat frame = cv::Mat(700, 1200, CV_8UC3);
	cvui::init(WINDOW_NAME);
	cv::namedWindow(WINDOW_NAME, WINDOW_FULLSCREEN);
	int count = 0;
	bool noisedelete = 0;
	bool afterimaged = 0;
	bool PerP = 0;
	int thold = 0;
	bool FaceDetected = 0;
	bool segmented = 0;
	float size = 1;
	bool basicchanged = 0;
	Size dsize;
	Mat img;
	Mat img4;
	Mat originalimg1;
	Mat originimg;
	Mat afterimg;
	while (true) {
		frame = cv::Scalar(255, 255, 187);
		if (cvui::button(frame, 0, 0, "Add New Picture"))
		{
			//count = 0;
			GetFile();
			count += 1;
			if (judge == 0) {
				return 0;
			}
			else
				cout << "选择成功！" << endl;
		}
		if (count == 1) {
			img = imread(_char, 1);
			while (img.cols > 700 || img.rows > 1200)
			{
				dsize = Size(img.cols * 4 / 5, img.rows * 4 / 5);
				resize(img, img, dsize);
			}
			originimg = img;
			cvui::image(frame, 10, 100, originimg);
			if (afterimaged)
				cvui::imshow("new window", afterimg);
		}
		if (cvui::button(frame, 140, 0, "Preprocessing"))
		{
			if (PerP == 1)
			{
				PerP = 0;
			}
			else
				PerP = 1;
		}
		if (PerP==1){
			if (cvui::button(frame, 140, 50, "Dilate")) {
				//获取自定义核
				Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
				Mat out;
				//进行膨胀操作
				dilate(originimg, out, element);
				afterimg = out;
				afterimaged = 1;
			}
			if (cvui::button(frame, 140, 25, "Erode")) {
				Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
				Mat out;
				erode(originimg, out, element);
				afterimg = out;
				afterimaged = 1;
			}

		}
		if (cvui::button(frame, 350, 0, "Segment"))
		{
			if (segmented == 1)
			{
				segmented = 0;
			}
			else
				segmented = 1;
		}
		if (segmented == 1) {
			if (cvui::button(frame, 350, 25, "RegionGrowing")) {
				cvtColor(originimg, originimg, COLOR_BGR2GRAY);
				RegionGrowing(originimg, afterimg, Point2f(round(originimg.cols / 2), round(originimg.rows / 2)), 30);
				afterimaged = 1;
			}
			if (cvui::button(frame, 350, 50, "Roberts")) {
				cvtColor(originimg, originimg, COLOR_BGR2GRAY);
				GaussianBlur(originimg, originimg, Size(3, 3),
					0, 0, BORDER_DEFAULT);
				afterimg = roberts(originimg);
				afterimaged = 1;
			}
			if (cvui::button(frame, 350, 75, "OTSU")) {
				cvtColor(originimg, originimg, COLOR_BGR2GRAY);
				thold = getOTSUthread(originimg,afterimg);
				threshold(originimg, afterimg, thold, 255, cv::THRESH_BINARY);
				//threshold(originimg, afterimg, 0, 255, THRESH_OTSU);
				//getOTSUthread(originimg, afterimg);
				afterimaged = 1;
			}
		}
		if (cvui::button(frame, 450, 0, "FaceDetection&Processing"))
		{
			if(FaceDetected == 1)
			{
				FaceDetected = 0;

			}
			else
				FaceDetected = 1;
		}
		if (FaceDetected == 1) {
			afterimg = originimg;
			DetectAndDisplay(afterimg);
			afterimaged = 1;
		}
		if (cvui::button(frame, 1000, 0, "Save"))
		{
			imwrite("result.jpg", afterimg);
		}
		cvui::imshow(WINDOW_NAME, frame);
		if (cv::waitKey(20) == 'q') {
			break;
		}
	}
	return 0;
}


