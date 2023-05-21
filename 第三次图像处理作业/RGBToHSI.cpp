#include "RGBToHSI.h"
Mat RGB2HSI(const Mat & rgb){
    Mat hsi(rgb.rows, rgb.cols, rgb.type());
    float  H=0, S=0, I=0;
    for(int i=0; i < rgb.rows; i++)
        for(int j=0; j < rgb.cols; j++){
            float B = rgb.at<Vec3b>(i, j)[0] / 255.f,
                  G = rgb.at<Vec3b>(i, j)[1] / 255.f,
                  R = rgb.at<Vec3b>(i, j)[2] / 255.f;

            float num = (R - G + R - B) / 2,
                  den = sqrt((R - G) * (R - G) + (R - B) * (G - B)),
                  theta = acos(num/den);
            if(den == 0) H = 0; // 分母不能为0
            else H = B <= G ? theta / (2 * MY_PI) : 1 - theta / (2 * MY_PI);

            float sum = B + G + R;
            if(sum == 0) S = 0;
            else S = 1 - 3 * min(min(B, G), R) / sum;

            I = sum/3.0;

            hsi.at<Vec3b>(i, j)[0] = H*255;
            hsi.at<Vec3b>(i, j)[1] = S*255;
            hsi.at<Vec3b>(i, j)[2] = I*255;
        }
    return hsi;
}
