#include <iostream>
using namespace std;
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<chrono>
#include<windows.h>
#include"open.c"
#include"Stitch.h"
#include"RGBToHSI.h"
using namespace cv;
#define WINDOW_NAME "简易图像处理"

int main()
{
    //char filename[50]="C:\\Users\\S\\Desktop\\cat.bmp";
    cv::Mat frame = cv::Mat(700, 1200, CV_8UC3);
    cvui::init(WINDOW_NAME);
    cv::namedWindow(WINDOW_NAME, WINDOW_FULLSCREEN);
    int count = 0;
    float size = 1;
    bool afterimaged = 0;
    int angle = 0;
    int top, bottom, left, right;
    Mat img;
    Mat img4;
    Size dsize;
    vector<Mat> imgs;
    Mat image1;
    Mat HSIIMAGE;
    Mat pano;
    bool conversed = 0;
    bool jointed = 0;
    Mat GrayImage;
    Mat BinaryImage;
    Mat originimg;
    Mat afterimg;
    while (true)
    {
        frame = cv::Scalar(255, 255, 187);
        if (cvui::button(frame, 0, 0, "Add New Picture"))
        {
            //count = 0;
            GetFile();
            count += 1;
            if (judge == 0)
            {
                return 0;
            }
            else
                cout << "选择成功！" << endl;
        }
        if (count == 1)
        {
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
        if (cvui::button(frame, 140, 0, "Type Conversion"))
        {
            if (conversed == 1)
            {
                conversed = 0;
            }
            else
                conversed = 1;
        }
        if (conversed)
        {
            if (cvui::button(frame, 140, 25, "RGB HSI"))
            {
                afterimg = RGB2HSI(originimg);
                //split(afterimg,vecHsi);
                afterimaged = 1;
            }
            if (cvui::button(frame, 140, 50, "Gray"))
            {
                afterimg.create(originimg.size(), originimg.type());
                //将原图转换为灰度图像
                cvtColor(originimg, afterimg, cv::COLOR_BGR2GRAY);
                //split(afterimg,vecHsi);
                afterimaged = 1;
            }
            if (cvui::button(frame, 140, 75, "Binary"))
            {
                GrayImage.create(originimg.size(), originimg.type());
                //将原图转换为灰度图像
                cvtColor(originimg, GrayImage, COLOR_BGR2GRAY);
                threshold(GrayImage, afterimg, 150, 255, cv::THRESH_BINARY);
                //split(afterimg,vecHsi);
                afterimaged = 1;
            }
        }
        if (cvui::button(frame, 280, 0, "Joint"))
        {
            if (jointed == 1)
            {
                jointed = 0;
            }
            else
                jointed = 1;
        }
        if (jointed)
        {
            if (cvui::button(frame, 280, 25, "Add Image"))
            {
                GetFile();
                img = imread(_char);
                while (img.cols > 700 || img.rows > 1200)
                {
                    dsize = Size(img.cols * 4 / 5, img.rows * 4 / 5);
                    resize(img, img, dsize);
                }
                imgs.push_back(img);
            }
            if (cvui::button(frame, 280, 50, "Stitcher"))
            {
                Ptr<Stitcher> stitcher = Stitcher::create();
                cv::Stitcher::Status status = stitcher->stitch(imgs, pano);
                if (status != cv::Stitcher::OK)
                {
                    cout << "Can't stitch images, error code = " << int(status) << endl;
                    //return -1;
                }
                afterimg = pano;
                afterimaged = 1;
                cvui::imshow("new window",afterimg );
            }
            if (cvui::button(frame, 280, 70, "ORB"))
            {
                afterimg=ORB222(imgs[0],imgs[1]);
                afterimaged = 1;
                cvui::imshow("new window",afterimg );
            }
        }
        if (cvui::button(frame, 1100 - 110, 0, "Save"))
        {
            imwrite("result.jpg", afterimg);
        }
        cvui::imshow(WINDOW_NAME, frame);
        if (cv::waitKey(20) == 'q')
        {
            break;
        }
    }

    return 0;
}
