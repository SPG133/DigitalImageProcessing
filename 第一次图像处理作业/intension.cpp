#include"intension.h"
#include<iostream>
#include<algorithm>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace std;
using namespace cv;

int contrastValue = 1;	// 对比度值
float contrastValue_f; // 实际使用的对比度值，对contrastValue进行的缩小处理
int brightValue = 1;   // 亮度值
int saturation = 1;	// 饱和度
const int max_increment = 200;
Mat img1, new_img;
void contrast_bright()
{
    contrastValue_f = 0.1 * contrastValue;
    for (int row = 0; row < img1.rows; row++)
    {
        for (int col = 0; col < img1.cols; col++)
        {
            for (int c = 0; c < 3; c++) {
                new_img.at<Vec3b>(row, col)[c] = saturate_cast<uchar>(contrastValue_f * (img1.at<Vec3b>(row, col)[c]) + brightValue);  // g(x,y) = af(x,y) + b; a 是对比度调节，b是亮度调节
            }
        }
    }
    //imshow("Effect Image", new_img);
}

//调整饱和度
void saturability()
{
    float increment = (saturation - 80) * 1.0 / max_increment;
    for (int col = 0; col < img1.cols; col++)
    {
        for (int row = 0; row < img1.rows; row++)
        {
            // R,G,B 分别对应数组中下标的 2,1,0
            uchar r = img1.at<Vec3b>(row, col)[2];
            uchar g = img1.at<Vec3b>(row, col)[1];
            uchar b = img1.at<Vec3b>(row, col)[0];

            float maxn = max(r, max(g, b));
            float minn = min(r, min(g, b));

            float delta, value;
            delta = (maxn - minn) / 255;
            value = (maxn + minn) / 255;

            float new_r, new_g, new_b;

            if (delta == 0)		 // 差为 0 不做操作，保存原像素点
            {
                //new_img.at<Vec3b>(row, col)[0] = new_b;
                //new_img.at<Vec3b>(row, col)[1] = new_g;
                //new_img.at<Vec3b>(row, col)[2] = new_r;
                continue;
            }

            float light, sat, alpha;
            light = value / 2;

            if (light < 0.5)
                sat = delta / value;
            else
                sat = delta / (2 - value);

            if (increment >= 0)
            {
                if ((increment + sat) >= 1)
                    alpha = sat;
                else
                {
                    alpha = 1 - increment;
                }
                alpha = 1 / alpha - 1;
                new_r = r + (r - light * 255) * alpha;
                new_g = g + (g - light * 255) * alpha;
                new_b = b + (b - light * 255) * alpha;
            }
            else
            {
                alpha = increment;
                new_r = light * 255 + (r - light * 255) * (1 + alpha);
                new_g = light * 255 + (g - light * 255) * (1 + alpha);
                new_b = light * 255 + (b - light * 255) * (1 + alpha);
            }
            new_img.at<Vec3b>(row, col)[0] = new_b;
            new_img.at<Vec3b>(row, col)[1] = new_g;
            new_img.at<Vec3b>(row, col)[2] = new_r;
        }
    }
}
    // imshow("Effect Image", new_img);
    Mat intensionupdate(Mat oldimg, int con, int bri, int sat){
        img1 =oldimg;
        new_img = Mat::zeros(img1.size(), img1.type());
        saturation = sat;
        contrastValue = con;
        brightValue = bri;
        contrast_bright();
        saturability();
        return new_img;
    }
