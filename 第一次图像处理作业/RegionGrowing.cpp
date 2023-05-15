#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <chrono>
#include "RegionGrowing.h"
using namespace std;
bool RegionGrowing(Mat img, Mat& result, Point2i seed, int threshold) {
	// 将图像全部设置为黑
	result = Mat::zeros(img.size(), CV_8UC1);
	// 设置种子点范围条件
	if (seed.x < 0 || seed.y < 0 || seed.y > img.rows - 1 || seed.x > img.cols - 1) {
		return false;
	}
	// 种子点集
	vector<Point2i> seeds;
	// 压入初始种子点
	seeds.push_back(seed);
	// 种子点设置为白
	result.ptr<uchar>(seed.y)[seed.x] = 255;
	// 8连接生长方向
	int growDirections[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1} };


	// 开始生长
	while (!seeds.empty()) {
		// 取出一个种子点作为现在循环的初始种子点
		Point2i seed_current = seeds.back();
		seeds.pop_back();


		// 遍历各生长方向的邻点
		for (int i = 0; i < 8; i++) {
			Point2i neighborPoint = { seed_current.x + growDirections[i][0], seed_current.y + growDirections[i][1] }; // 邻点
			if (neighborPoint.x < 0 || neighborPoint.y < 0 || neighborPoint.x > img.cols - 1 || neighborPoint.y > img.rows - 1) { // 邻点超出范围
				continue;
			}
			if ((result.ptr<uchar>(neighborPoint.y)[neighborPoint.x] == 0) && abs(img.ptr<uchar>(neighborPoint.y)[neighborPoint.x] - img.ptr<uchar>(seed.y)[seed.x]) < threshold) {
				// 设置为种子点
				result.ptr<uchar>(neighborPoint.y)[neighborPoint.x] = 255; // 设置为白色
				seeds.push_back(neighborPoint); // 压入种子集
			}
		}
	}


	return true;
}
