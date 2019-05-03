#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
using namespace cv;
using namespace std;

#include"MouseDraw.h"
#include"Features.h"

void getSegment(const Mat& srcImage, vector<Mat>& arr, Mat& showImage);
bool cmp(const Rect& a, const Rect& b);
bool cmp2(const Rect& a, const Rect& b);