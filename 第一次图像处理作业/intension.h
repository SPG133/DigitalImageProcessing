#pragma once
#include<iostream>
#include<algorithm>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\types_c.h>

using namespace std;
using namespace cv;
void contrast_bright();
void saturability();
Mat intensionupdate(Mat, int, int, int);