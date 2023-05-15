#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <chrono>
#include "RegionGrowing.h"
using namespace std;
bool RegionGrowing(Mat img, Mat& result, Point2i seed, int threshold) {
	// ��ͼ��ȫ������Ϊ��
	result = Mat::zeros(img.size(), CV_8UC1);
	// �������ӵ㷶Χ����
	if (seed.x < 0 || seed.y < 0 || seed.y > img.rows - 1 || seed.x > img.cols - 1) {
		return false;
	}
	// ���ӵ㼯
	vector<Point2i> seeds;
	// ѹ���ʼ���ӵ�
	seeds.push_back(seed);
	// ���ӵ�����Ϊ��
	result.ptr<uchar>(seed.y)[seed.x] = 255;
	// 8������������
	int growDirections[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1} };


	// ��ʼ����
	while (!seeds.empty()) {
		// ȡ��һ�����ӵ���Ϊ����ѭ���ĳ�ʼ���ӵ�
		Point2i seed_current = seeds.back();
		seeds.pop_back();


		// ����������������ڵ�
		for (int i = 0; i < 8; i++) {
			Point2i neighborPoint = { seed_current.x + growDirections[i][0], seed_current.y + growDirections[i][1] }; // �ڵ�
			if (neighborPoint.x < 0 || neighborPoint.y < 0 || neighborPoint.x > img.cols - 1 || neighborPoint.y > img.rows - 1) { // �ڵ㳬����Χ
				continue;
			}
			if ((result.ptr<uchar>(neighborPoint.y)[neighborPoint.x] == 0) && abs(img.ptr<uchar>(neighborPoint.y)[neighborPoint.x] - img.ptr<uchar>(seed.y)[seed.x]) < threshold) {
				// ����Ϊ���ӵ�
				result.ptr<uchar>(neighborPoint.y)[neighborPoint.x] = 255; // ����Ϊ��ɫ
				seeds.push_back(neighborPoint); // ѹ�����Ӽ�
			}
		}
	}


	return true;
}
