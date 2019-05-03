#include"pch.h"
#include"Recognize.h"
#include"BPNeuronNet.h"

#define WINDOW_NAME "predict"
void handWritingRecognize(BPNeuronNet& bpNeuronNet) {
	Mat srcImage(500, 500, CV_8UC1, Scalar::all(0)), image;
	namedWindow(WINDOW_NAME);
	while (true) {
		BoxExtractor box;
		srcImage.copyTo(image);
		int key = box.MouseDraw(WINDOW_NAME, image, Scalar::all(255), 5);
		if (key != 13) {
			break;
		}
		vector<Mat> arr;
		getSegment(image, arr, image);
		//Mat imageDataMat;
		cout << "预测结果" << endl;
		//float ans = -1;
		for (int i = 0; i < arr.size(); ++i) {
			//Mat imageDataMat;
			//getMixedFeature(arr[i], imageDataMat, Size(28, 28));
			//predict
			resize(arr[i], arr[i], Size(20, 20));//归一化
			copyMakeBorder(arr[i], arr[i], 4, 4, 4, 4, BORDER_CONSTANT, Scalar::all(0));//边界填充，填充纯黑边界
			uchar **array = new uchar*[arr[i].rows];
			for (int j = 0; j < arr[i].rows; j++) {
				array[j] = new uchar[arr[i].cols];
			}
			for (int j = 0; j < arr[i].rows; j++) {
				for (int k = 0; k < arr[i].cols; k++) {
					array[j][k] = arr[i].at<uchar>(j, k);
				}
			}
			double netTest[28 * 28];
			double * netOut = nullptr;
			for (int j = 0; j < 28; j++) {
				for (int k = 0; k < 28; k++) {
					netTest[j * 28 + k] = (double)array[j][k] > 0 ? 1.0 : 0.0;
					//cout << netTest[j * 28 + k];
				}
				//cout << endl;
			}
			bpNeuronNet.predict(netTest, &netOut);
			int id = -1;
			double maxValue = -999;
			for (int j = 0; j < 10; j++) {
				if (netOut[j] > maxValue) {
					maxValue = netOut[j];
					id = j;
				}
			}
			cout << id << " ";
		}
		cout << endl;
		imshow(WINDOW_NAME, image);
		cout << "输入任意字符进行下一次的输入，输入esc回到上一级" << endl;
		key = waitKey(0);
		if (key == 27) {
			break;
		}
		else {
			continue;
		}
		cout << endl;
			
	}
	destroyWindow(WINDOW_NAME);
}