#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <opencv2/objdetect.hpp>
#include <Windows.h>
#include <opencv2/imgproc.hpp>
#include <ShlObj.h>
#include <chrono>
using namespace cv;
using namespace std;
int DetectAndDisplay(Mat& frame);
