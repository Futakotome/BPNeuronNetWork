#include "pch.h"
#include <iostream>
#include<fstream>
#include<future>
#include<sys/timeb.h>
#include"BPNeuronNet.h"
#include"NeuronUtils.h"
#include"Image.h"
#include"ProgressBar.h"
#include"Recognize.h"


#define TRAIN_IMAGES_URL "train-images.idx3-ubyte"
#define TRAIN_LABELS_URL "train-labels.idx1-ubyte"
#define TEST_IMAGES_URL "t10k-images.idx3-ubyte"
#define TEST_LABELS_URL "t10k-labels.idx1-ubyte"

#define big_to_little32(A) ((((uint32_t)(A) & 0xff000000)>>24)|(((uint32_t)(A) & 0x00ff0000)>>8)|(((uint32_t)(A) & 0x0000ff00)<<8)|(((uint32_t)(A) & 0x000000ff)<<24))
#define load_from_big_to_little32(istream,value) istream.read(reinterpret_cast<char*>(&value),sizeof value);value=big_to_little32(value)

typedef struct List {
	Image * imageList;
	int lenght;
}ImageList;

ProgressBar progressBar(70000, "loading test and train image...");
ImageList loadImageAndLabelFile(const std::string& imagePath, const std::string& labelPath);
void loadImageAndLabelFileInThread(std::promise<ImageList> &promiseObj, const std::string& imagePath, const std::string& labelPath);
void reset(const std::string& imagePath, const std::string& labelPath);
long long getCurrentTime();
void preProcessInputDataWithNoise(const uint8_t *pixel, double *out, int size);
void preProcessInputData(const uint8_t *pixel, double *out, int size);
double train(const ImageList& trainList, BPNeuronNet& bpNeuronNet);
int test(const ImageList& testList, BPNeuronNet& bpNeuronNet);


int main()
{
	using namespace std;
	promise<ImageList> trainingObj;
	future<ImageList> trainFutureObj = trainingObj.get_future();

	promise<ImageList> testObj;
	future<ImageList> testFutureObj = testObj.get_future();

	thread t1(loadImageAndLabelFileInThread, ref(trainingObj), TRAIN_IMAGES_URL, TRAIN_LABELS_URL);
	thread t2(loadImageAndLabelFileInThread, ref(testObj), TEST_IMAGES_URL, TEST_LABELS_URL);

	ImageList trainingImageList = trainFutureObj.get();
	ImageList testImageList = testFutureObj.get();

	t1.join();
	t2.join();

	auto startTime = getCurrentTime();
	BPNeuronNet bpNeuronNet(trainingImageList.imageList[0].getRow()*trainingImageList.imageList[0].getColumn(), 0.1);
	//bpNeuronNet.addLayer(784);
	bpNeuronNet.addLayer(100);
	bpNeuronNet.addLayer(10);
	double error = train(trainingImageList, bpNeuronNet);
	//while (error > 0.01) {
	//	error = train(trainingImageList, bpNeuronNet);
	//	cout << "now error rate:" << error << endl;
	//	reset(TRAIN_IMAGES_URL, TRAIN_LABELS_URL);
	//}

	cout << "error rate: " << error << endl;
	cout << "time:" << getCurrentTime() - startTime << endl;
	auto success = test(testImageList, bpNeuronNet);
	cout << "success:" << success << "cout:" << testImageList.lenght << endl;
	cout << "time:" << getCurrentTime() - startTime << endl;

	cout << "手写数字识别测试" << endl;
	handWritingRecognize(bpNeuronNet);

}
void reset(const std::string& imagePath, const std::string& labelPath){
	using namespace std;
	ifstream digitalImage;
	ifstream digitalLabel;
	
	digitalImage.open(imagePath, ios::binary);
	digitalLabel.open(labelPath, ios::binary);

	if (digitalImage.is_open()) {
		digitalImage.clear();
		digitalImage.seekg(0);
	}
	if (digitalLabel.is_open()) {
		digitalLabel.clear();
		digitalLabel.seekg(0);
	}
}
void preProcessInputDataWithNoise(const uint8_t *pixel, double *out, int size) {
	for (int i = 0; i < size; i++) {
		out[i] = ((pixel[i] > 0) ? 1.0 : 0.0);
	}
}
void preProcessInputData(const uint8_t *pixel, double *out, int size) {
	for (int i = 0; i < size; i++) {
		out[i] = ((pixel[i] > 0) ? 1.0 : 0.0);
	}
}
long long getCurrentTime() {
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}
ImageList loadImageAndLabelFile(const std::string& imagePath, const std::string& labelPath) {
	using namespace std;
	ImageList list = {};
	ifstream digitalImage;
	ifstream digitalLabel;

	digitalLabel.open(labelPath, ios::binary);
	digitalImage.open(imagePath, ios::binary);

	int32_t imageMagicNumber, imageCount, rows, columns;
	int32_t labelMagicNumber, labelCount;
	if (digitalImage.is_open() && digitalLabel.is_open()) {
		load_from_big_to_little32(digitalLabel, labelMagicNumber);
		load_from_big_to_little32(digitalLabel, labelCount);

		load_from_big_to_little32(digitalImage, imageMagicNumber);
		load_from_big_to_little32(digitalImage, imageCount);
		load_from_big_to_little32(digitalImage, rows);
		load_from_big_to_little32(digitalImage, columns);
		if (imageCount != labelCount) {
			digitalImage.close();
			digitalLabel.close();
			throw exception("input file have some error");
		}
		Image * imageList = new Image[imageCount];
		for (int i = 0; i < imageCount; i++) {
			imageList[i].initImage(rows, columns);
		}
		for (int i = 0; i < imageCount; i++) {
			for (int x = 0; x < rows; x++) {
				for (int y = 0; y < columns; y++) {
					uint8_t grayScale;
					digitalImage.read(reinterpret_cast<char *>(&grayScale), sizeof grayScale);
					imageList[i].setPixel(x, y, grayScale);
				}
			}
			uint8_t label;
			digitalLabel.read(reinterpret_cast<char *>(&label), sizeof label);
			imageList[i].setNumber(label);

			++progressBar;
		}
		list.imageList = imageList;
		list.lenght = imageCount;
	}
	digitalImage.close();
	digitalLabel.close();
	return list;
}
void loadImageAndLabelFileInThread(std::promise<ImageList> &promiseObj, const std::string& imagePath, const std::string& labelPath) {
	promiseObj.set_value(loadImageAndLabelFile(imagePath, labelPath));

}

double train(const ImageList& trainList, BPNeuronNet& bpNeuronNet) {
	double netTarget[10];
	double netTrain[28 * 28];
	progressBar.reset(trainList.lenght, "start training...");
	for (int i = 0; i < trainList.lenght; i++) {
		memset(netTarget, 0, 10 * sizeof(double));
		netTarget[trainList.imageList[i].number] = 1.0;
		//给样本加入随机噪声
		preProcessInputDataWithNoise(trainList.imageList[i].getPixel(), netTrain, 28 * 28);
		bpNeuronNet.training(netTrain, netTarget);
		++progressBar;
	}
	return bpNeuronNet.getError();
}
int test(const ImageList& testList, BPNeuronNet& bpNeuronNet) {
	int success = 0;
	double * netOut = nullptr;
	double netTest[28 * 28];
	progressBar.reset(testList.lenght, "testing neuron net...");
	for (int i = 0; i < testList.lenght; i++) {
		preProcessInputData(testList.imageList[i].getPixel(), netTest, 28 * 28);
		bpNeuronNet.predict(netTest, &netOut);

		int id = -1;
		double maxValue = -999;
		for (int j = 0; j < 10; j++) {
			if (netOut[j] > maxValue) {
				maxValue = netOut[j];
				id = j;
			}
		}
		if (id == testList.imageList[i].number) {
			success++;
		}
		++progressBar;
	}
	return success;
}
