#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <chrono>
using namespace cv;
Mat gaussianlbrf(Mat , float );
Mat freqfilt(Mat , Mat );