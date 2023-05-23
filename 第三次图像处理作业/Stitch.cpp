﻿#include "Stitch.h"

//1、使用特征检测算法找到两张图像中相似的点，计算变换矩阵
//2、将图像right透视变换后得到的图片与图像left拼接
 
#include <iostream>  
#include <opencv2/opencv.hpp>
#include <opencv.hpp>
 
 
using namespace cv;
using namespace std;
 
 
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst,four_corners_t);
 

 

 
void CalcCorners(const Mat& H, const Mat& src,four_corners_t &corners)
{
    double v2[] = { 0, 0, 1 };//左上角
    double v1[3];//变换后的坐标值
    Mat V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    Mat V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
 
    V1 = H * V2;
    //左上角(0,0,1)
    cout << "V2: " << V2 << endl;
    cout << "V1: " << V1 << endl;
    corners.left_top.x = v1[0] / v1[2];
    corners.left_top.y = v1[1] / v1[2];
 
    //左下角(0,src.rows,1)
    v2[0] = 0;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.left_bottom.x = v1[0] / v1[2];
    corners.left_bottom.y = v1[1] / v1[2];
    cout<<"dwdawd"<<endl;
    //右上角(src.cols,0,1)
    v2[0] = src.cols;
    v2[1] = 0;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.right_top.x = v1[0] / v1[2];
    corners.right_top.y = v1[1] / v1[2];
 
    //右下角(src.cols,src.rows,1)
    v2[0] = src.cols;
    v2[1] = src.rows;
    v2[2] = 1;
    V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
    V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
    V1 = H * V2;
    corners.right_bottom.x = v1[0] / v1[2];
    corners.right_bottom.y = v1[1] / v1[2];
 
}
Mat ORB222(Mat imageRight, Mat imageLeft)
{
    four_corners_t corners;
    //Mat imageRight = imread("images/imgR.jpg", 1);    //右图
    //Mat imageLeft = imread("images/imgL.jpg", 1);    //左图
 
    //Mat imageRight = imread("images/SR.jpg", 1);    //右图
    //Mat imageLeft = imread("images/SL.jpg", 1);    //左图
    
 
    //灰度图转换  
    Mat image_r, image_l;
    cvtColor(imageRight, image_r, COLOR_BGR2GRAY);
    cvtColor(imageLeft, image_l, COLOR_BGR2GRAY);
 
    //直接从可能重复的区域提取特征点匹配 当前是左右图在拼接处大概有1/3是重复的
    Rect rect_right = Rect(0, 0, imageRight.cols / 3, imageRight.rows);
    Rect rect_left = Rect(2*imageLeft.cols/3, 0, (imageLeft.cols/3) -1 , imageLeft.rows);
 
    Mat image_r_rect = imageRight(Rect(rect_right));
    Mat image_l_rect = imageLeft(Rect(rect_left));
 
 
    
 
    //提取特征点    
    Ptr<FeatureDetector> ORBDetector = ORB::create(10000);
 
    vector<KeyPoint> keyPoints_r, keyPoints_l;
    ORBDetector->detect(image_r_rect, keyPoints_r);
    ORBDetector->detect(image_l_rect, keyPoints_l);
 
   
  cout<<"dadawdaw";
    //特征点描述，为下边的特征点匹配做准备    
    Ptr<DescriptorExtractor> ORBDescriptor = ORB::create(10000);
 
    Mat imageDesc_r, imageDesc_l;
 
 
    ORBDescriptor->compute(image_r_rect, keyPoints_r, imageDesc_r);
    ORBDescriptor->compute(image_l_rect, keyPoints_l, imageDesc_l);
 
   cout<<"011111"<<endl;
 
    flann::Index flannIndex(imageDesc_r, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);
 
    vector<DMatch> GoodMatchePoints;
 
    Mat macthIndex(imageDesc_l.rows, 2, CV_32F), matchDistance(imageDesc_l.rows, 2, CV_32F);
    flannIndex.knnSearch(imageDesc_l, macthIndex, matchDistance, 2, flann::SearchParams());
    cout<<"dadawdaw1";
    // Lowe's algorithm,获取优秀匹配点
    for (int i = 0; i < matchDistance.rows; i++)
    {
        if (matchDistance.at<float>(i, 0) < 0.4 * matchDistance.at<float>(i, 1))
        {
            DMatch dmatches(i, macthIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
            GoodMatchePoints.push_back(dmatches);
        }
    }
 
 
 
    Mat first_match;
    //drawMatches(imageLeft, keyPoints_l, imageRight, keyPoints_r, GoodMatchePoints, first_match);
    drawMatches(image_l_rect, keyPoints_l, image_r_rect, keyPoints_r, GoodMatchePoints, first_match);
    cout<<"dadawdaw2";
    //namedWindow("first_match ", 2);
    //imshow("first_match ", first_match);
    //waitKey();
 
    vector<Point2f> imagePoints1, imagePoints2;
 
    for (int i = 0; i < GoodMatchePoints.size(); i++)
    {
        imagePoints2.push_back(keyPoints_l[GoodMatchePoints[i].queryIdx].pt);
        imagePoints1.push_back(keyPoints_r[GoodMatchePoints[i].trainIdx].pt);
    }
 
    if (imagePoints1.size() <= 10 || imagePoints2.size() <= 10)
    {
        printf("There is little keypoints\n");
    }
 
    //将左图的坐标转化到原图的位置，否则其变换矩阵在x方向的平稳不对
 
    cout<<"test03"<<endl;
    //获取图像1到图像2的投影映射矩阵 尺寸为3*3  
    Mat homo = findHomography(imagePoints1, imagePoints2, RANSAC);
   // 也可以使用getPerspectiveTransform方法获得透视变换矩阵，不过要求只能有4个点，效果稍差
    //Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
    //Mat   homo=getPerspectiveTransform(imagePoints1,imagePoints2);  
      cout << "变换矩阵为：\n" << homo << endl << endl; //输出映射矩阵      
          
     //计算配准图的四个顶点坐标
    CalcCorners(homo, imageRight,corners);
 
    //cout << "left_top:" << corners.left_top << endl;
    //cout << "left_bottom:" << corners.left_bottom << endl;
    //cout << "right_top:" << corners.right_top << endl;
    //cout << "right_bottom:" << corners.right_bottom << endl;
 
 
    //图像配准  
    Mat imageTransform1, imageTransform2;
    warpPerspective(imageRight, imageTransform1, homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), imageLeft.rows));
    cout<<"007"<<endl;
    rectangle(imageRight, Rect(imageRight.cols - MAX(corners.right_top.x, corners.right_bottom.x), 0, MAX(corners.right_top.x, corners.right_bottom.x), 500), (0, 0, 255));
    //imshow("透视矩阵变换Right", imageTransform1);
    //imwrite("trans1.jpg", imageTransform1);
    //waitKey();
    cout<<"test06"<<endl;
 
    //创建拼接后的图,需提前计算图的大小
    //int move_x = 900;
    int dst_width = imageTransform1.cols;  //取最右点的长度为拼接图的长度
 
    int dst_height = imageLeft.rows;
    cout<<"test05"<<endl;
    Mat dst(dst_height, dst_width, CV_8UC3);
    dst.setTo(0);
    cout<<"test04"<<endl;
    imageTransform1.copyTo(dst(Rect(0, 0, imageTransform1.cols, imageTransform1.rows)));
    cout<<"dwaedw";
    imageLeft.copyTo(dst(Rect(0, 0, imageLeft.cols, imageLeft.rows)));
    cout<<"dwaedw";
    //OptimizeSeam(imageLeft, imageTransform1, dst,corners);
 
    //imshow("dstOptimize", dst);
    //imwrite("dst.jpg", dst);

    return dst;

}
 
 
//优化两图的连接处，使得拼接自然
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst,four_corners_t corners)
{
    int start = MIN(corners.left_top.x, corners.left_bottom.x);//开始位置，即重叠区域的左边界  
 
    double processWidth = img1.cols - start;//重叠区域的宽度  
    int rows = dst.rows;
    int cols = img1.cols; //注意，是列数*通道数
    double alpha = 1;//img1中像素的权重  
    for (int i = 0; i < rows; i++)
    {
        uchar* p = img1.ptr<uchar>(i);  //获取第i行的首地址
        uchar* t = trans.ptr<uchar>(i);
        uchar* d = dst.ptr<uchar>(i);
        for (int j = start; j < cols; j++)
        {
            //如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
            if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
            {
                alpha = 1;
            }
            else
            {
                //img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
                alpha = (processWidth - (j - start)) / processWidth;
            }
 
            d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
            d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
            d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
 
        }
    }
 
}
 
