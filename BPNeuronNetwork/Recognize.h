#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include"Segment.h"
#include"BPNeuronNet.h"
using namespace cv;
using namespace cv::ml;
using namespace std;
void handWritingRecognize(BPNeuronNet& bpNeuronNet);
