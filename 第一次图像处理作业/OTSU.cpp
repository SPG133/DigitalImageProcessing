#include"OTSU.h"
using namespace std;
//using namespace cv;
int getOTSUthread(Mat grayImg, Mat& binImg)
{
    int rows = grayImg.rows;
    int cols = grayImg.cols;
    int sumPix = rows * cols;
    // �Ҷ�ֱ��ͼ
    vector<int> grayHist;
    for (int i = 0; i < 256; i++)
    {
        grayHist.push_back(0);
    }
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int gray = grayImg.at<uchar>(r, c);
            grayHist[gray]++;
        }
    }
    float sum = 0;
    for (int i = 0; i < 256; i++)
    {
        sum += i * grayHist[i];
    }
    float sumB = 0;
    int wB = 0;
    int wF = 0;

    float varMax = 0;
    int thresh = 0;
    for (int i = 0; i < 256; i++)
    {
        wB += grayHist[i];      // ����Ȩ��
        if (wB == 0)
        {
            continue;
        }
        wF = sumPix - wB;       // ǰ��Ȩ��
        if (wF == 0)
        {
            break;
        }
        sumB += (float)(i * grayHist[i]);
        float mB = sumB / wB;           // ������ֵ
        float mF = (sum - sumB) / wF;   //ǰ����ֵ
        float varTmp = (float)wB / (float)sumPix * (float)wF / (float)sumPix * (mB - mF) * (mB - mF); // ���㷽��
        if (varTmp > varMax)
        {
            varMax = varTmp;
            thresh = i;
        }
    }
    // ��ֵ����
    return thresh;
}
