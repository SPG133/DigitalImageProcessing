#include"Roberts.h"
Mat roberts(Mat srcImage)
{
	Mat dstImage = srcImage.clone();
	int nRows = dstImage.rows;
	int nCols = dstImage.cols;
	for (int i = 0; i < nRows - 1; i++) {
		for (int j = 0; j < nCols - 1; j++) {
			//���ݹ�ʽ����
			int t1 = (srcImage.at<uchar>(i, j) -
				srcImage.at<uchar>(i + 1, j + 1)) *
				(srcImage.at<uchar>(i, j) -
					srcImage.at<uchar>(i + 1, j + 1));
			int t2 = (srcImage.at<uchar>(i + 1, j) -
				srcImage.at<uchar>(i, j + 1)) *
				(srcImage.at<uchar>(i + 1, j) -
					srcImage.at<uchar>(i, j + 1));
			//����g��x,y��
			dstImage.at<uchar>(i, j) = (uchar)sqrt(t1 + t2);
		}
	}
	return dstImage;
}

