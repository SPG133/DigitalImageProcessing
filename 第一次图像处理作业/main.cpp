#include <iostream>
using namespace std;
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<chrono>
#include<windows.h>
#include"open.c"
#include"basicchange.h"
#include"Gaussian.h"
#include"intension.h"
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
	bool intension = 0;
	float size = 1;
	bool basicchanged = 0;
	Size dsize;
	int angle = 0;
	int top, bottom, left, right;
	bool stymmetry = 0;
	Mat img;
	Mat img4;
	Mat originalimg1;
	float maxsize = 1;
	float minsize = 1;
	int contrastValue1 = 1;	// 对比度值
	int brightValue1 = 1;   // 亮度值
	int saturation1 = 10;	// 饱和度
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
		if (cvui::button(frame, 140, 0, "Add Noise"))
		{
			Mat img_noise = Mat::zeros(originimg.rows, originimg.cols, originimg.type());
			RNG rng;//创建一个RNG类
			rng.fill(img_noise, RNG::NORMAL, 10, 20);//生成高斯分布随机数
			Mat afteraddnoiseimg = originimg + img_noise;
			afterimg = afteraddnoiseimg;
			afterimaged = 1;
		}
		if (cvui::button(frame, 450 - 100, 0, "Basic Change"))
		{
			if (basicchanged == 1)
			{
				basicchanged = 0;
			}
			else
				basicchanged = 1;
		}
		if (basicchanged)
		{
			cvui::window(frame, 450 - 100, 25, 240, 200, "Settings");
			cvui::text(frame, 450 - 100, 50, "angle");
			cvui::trackbar(frame, 510 - 100, 50, 165, &angle, 0, 360);
			cvui::checkbox(frame, 470 - 100, 200, "Use Stymmetry", &stymmetry);
			cvui::text(frame, 450 - 100, 150, "maxsize");
			cvui::trackbar(frame, 510 - 100, 150, 165, &maxsize, (float)1, (float)2);
			cvui::text(frame, 450 - 100, 100, "minsize");
			cvui::trackbar(frame, 510 - 100, 100, 165, &minsize, (float)1, (float)3);
			size = maxsize / minsize;
			afterimg = basicchange(stymmetry, originimg, angle, size);
			afterimaged = 1;
			//basicchanged = 0;
		}
		if (cvui::button(frame, 300 - 60, 0, "Noise Delete"))
		{
			if (noisedelete == 1)
				noisedelete = 0;
			else
				noisedelete = 1;
		}
		if (noisedelete)
		{
			if (cvui::button(frame, 300 - 60, 25, "Gaussian Low-pass Filtering"))
			{
				cvtColor(originimg, originimg, COLOR_BGR2GRAY);
				int w = getOptimalDFTSize(originimg.cols);  //获取进行dtf的最优尺寸
				int h = getOptimalDFTSize(originimg.rows); //获取进行dtf的最优尺寸

				Mat padded;
				copyMakeBorder(originimg, padded, 0, h - originimg.rows, 0, w - originimg.cols, BORDER_CONSTANT, Scalar::all(0));  //边界填充
				padded.convertTo(padded, CV_32FC1); //将图像转换为flaot型

				Mat gaussianKernel = gaussianlbrf(padded, 45);//高斯低通滤波器

				afterimg = freqfilt(padded, gaussianKernel);
				afterimaged = 1;
			}
			if (cvui::button(frame, 300 - 60, 50, "Medianfiltering"))
			{
				medianBlur(originimg, afterimg, 3);
				afterimaged = 1;
			}
			//afterimaged = 1;

		}
		if (cvui::button(frame, 600 - 130, 0, "Image Intensifer"))
		{
			if (intension == 1)
			{
				intension = 0;
			}
			else
				intension = 1;
		}
		if (intension) {

			cvui::window(frame, 600 - 130, 25, 240, 180, "Settings");
			cvui::text(frame, 600 - 130, 50, "contrastValue");
			cvui::trackbar(frame, 660 - 130, 50, 165, &contrastValue1, 1, 200);
			cvui::text(frame, 600 - 130, 150, "brightValue");
			cvui::trackbar(frame, 660 - 130, 150, 165, &brightValue1, 1, 100);
			cvui::text(frame, 600 - 130, 100, "saturation");
			cvui::trackbar(frame, 660 - 130, 100, 165, &saturation1, 10, 200);
			afterimg = intensionupdate(originimg, contrastValue1, brightValue1, saturation1);
			afterimaged = 1;
		}
		if (cvui::button(frame, 750 - 140, 0, "Histogram Equalization"))
		{
			Mat hsv, dst[3], src;
			cvtColor(originimg, hsv, COLOR_BGR2HSV);
			split(hsv, dst);

			equalizeHist(dst[2], dst[2]);
			merge(dst, 3, src);

			cvtColor(src, src, COLOR_HSV2BGR);
			afterimg = src;
			afterimaged = 1;
		}
		if (cvui::button(frame, 900 - 110, 0, "Add Frame"))
		{
			top = (int)(0.05 * originimg.rows); bottom = (int)(0.05 * originimg.rows);
			left = (int)(0.05 * originimg.cols); right = (int)(0.05 * originimg.cols);
			cv::copyMakeBorder(originimg, afterimg, top, bottom, left, right, BORDER_CONSTANT, 2222);
			afterimaged = 1;
		}
		if (cvui::button(frame, 1000 - 110, 0, "Joint"))
		{
			GetFile();
			count += 1;
		}
		if (count == 2) {
			img = imread(_char, 1);
			while (img.cols > 700 || img.rows > 1200)
			{
				dsize = Size(img.cols * 4 / 5, img.rows * 4 / 5);
				resize(img, img, dsize);
			}
			int width = img.cols + originimg.cols;
			int height = max(img.rows, originimg.rows);
			afterimg = Mat(height, width, CV_8UC3, Scalar::all(0));
			Mat ROI_1 = afterimg(Rect(0, 0, originimg.cols, originimg.rows));
			Mat ROI_2 = afterimg(Rect(originimg.cols, 0, img.cols, img.rows));
			originimg.copyTo(ROI_1);
			img.copyTo(ROI_2);
			//afterimg = originimg;
			cvui::imshow("new window", afterimg);
		}
		if (cvui::button(frame, 1100 - 110, 0, "Save"))
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


