#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <iostream>
#include <chrono>
#include "Gaussian.h"
using namespace cv;
Mat gaussianlbrf(Mat scr, float sigma)
{
	Mat gaussianBlur(scr.size(), CV_32FC1); //��CV_32FC1
	float d0 = 2 * sigma * sigma;//��˹����������ԽС��Ƶ�ʸ�˹�˲���Խխ���˳���Ƶ�ɷ�Խ�࣬ͼ���Խƽ��
	for (int i = 0; i < scr.rows; i++)
	{
		for (int j = 0; j < scr.cols; j++)
		{
			float d = pow(float(i - scr.rows / 2), 2) + pow(float(j - scr.cols / 2), 2);//����,����pow����Ϊfloat��
			gaussianBlur.at<float>(i, j) = expf(-d / d0);//expfΪ��eΪ�����ݣ�����Ϊfloat�ͣ�
		}
	}
	imshow("��˹��ͨ�˲���", gaussianBlur);
	return gaussianBlur;
}


//*****************Ƶ�����˲�*******************
Mat freqfilt(Mat scr, Mat blur)
{
	//***********************DFT*******************
	Mat plane[] = { scr, Mat::zeros(scr.size() , CV_32FC1) }; //����ͨ�����洢dft���ʵ�����鲿��CV_32F������Ϊ��ͨ������
	Mat complexIm;
	merge(plane, 2, complexIm);//�ϲ�ͨ�� ������������ϲ�Ϊһ��2ͨ����Mat��������
	dft(complexIm, complexIm);//���и���Ҷ�任���������������  

	//***************���Ļ�********************
	split(complexIm, plane);//����ͨ����������룩

	int cx = plane[0].cols / 2; int cy = plane[0].rows / 2;//���µĲ������ƶ�ͼ��  (��Ƶ�Ƶ�����)
	Mat part1_r(plane[0], Rect(0, 0, cx, cy));  //Ԫ�������ʾΪ(cx,cy)
	Mat part2_r(plane[0], Rect(cx, 0, cx, cy));
	Mat part3_r(plane[0], Rect(0, cy, cx, cy));
	Mat part4_r(plane[0], Rect(cx, cy, cx, cy));

	Mat temp;
	part1_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp);  //���������½���λ��(ʵ��)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	Mat part1_i(plane[1], Rect(0, 0, cx, cy));  //Ԫ������(cx,cy)
	Mat part2_i(plane[1], Rect(cx, 0, cx, cy));
	Mat part3_i(plane[1], Rect(0, cy, cx, cy));
	Mat part4_i(plane[1], Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //���������½���λ��(�鲿)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp);  //���������½���λ��(�鲿)
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);

	//*****************�˲���������DFT����ĳ˻�****************
	Mat blur_r, blur_i, BLUR;
	multiply(plane[0], blur, blur_r); //�˲���ʵ�����˲���ģ���ӦԪ����ˣ�
	multiply(plane[1], blur, blur_i);//�˲����鲿���˲���ģ���ӦԪ����ˣ�
	Mat plane1[] = { blur_r, blur_i };
	merge(plane1, 2, BLUR);//ʵ�����鲿�ϲ�

	  //*********************�õ�ԭͼƵ��ͼ***********************************
	magnitude(plane[0], plane[1], plane[0]);//��ȡ����ͼ��0ͨ��Ϊʵ��ͨ����1Ϊ�鲿����Ϊ��ά����Ҷ�任����Ǹ���  
	plane[0] += Scalar::all(1);  //����Ҷ��o�����ͼƬ���÷��������ж�����������ȽϺÿ� 
	log(plane[0], plane[0]);    // float�͵ĻҶȿռ�Ϊ[0��1])
	cv::normalize(plane[0], plane[0], 1, 0, NORM_MINMAX);  //��һ��������ʾ
	imshow("ԭͼ��Ƶ��ͼ", plane[0]);


	//******************IDFT*******************************
	/*
	Mat part111(BLUR,Rect(0,0,cx,cy));  //Ԫ������(cx,cy)
	Mat part222(BLUR,Rect(cx,0,cx,cy));
	Mat part333(BLUR,Rect(0,cy,cx,cy));
	Mat part444(BLUR,Rect(cx,cy,cx,cy));
	 part111.copyTo(temp);  //���������½���λ��(�鲿)
	part444.copyTo(part111);
	temp.copyTo(part444);

	part222.copyTo(temp);  //���������½���λ��
	part333.copyTo(part222);
	temp.copyTo(part333);
	*/

	idft(BLUR, BLUR);	//idft���ҲΪ����
	split(BLUR, plane);//����ͨ������Ҫ��ȡͨ��
	magnitude(plane[0], plane[1], plane[0]);  //���ֵ(ģ)
	normalize(plane[0], plane[0], 1, 0, NORM_MINMAX);  //��һ��������ʾ
	return plane[0];//���ز���
}
