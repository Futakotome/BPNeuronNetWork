#include"pch.h"
#include"Segment.h"
bool cmp(const Rect& a, const Rect& b) {
	return a.x < b.x;
}
bool cmp2(const Rect& a, const Rect& b) {
	return a.tl().y < b.tl().y;
}
void sortRect(vector<Rect>& arr) {
	sort(arr.begin(), arr.end(), cmp2);
	vector<Rect>::iterator s = arr.begin();
	vector<Rect>::iterator e = arr.end();
	vector<Rect>::iterator ptr = arr.begin();
	vector<Rect>::iterator preptr = ptr++;
	for (;; ++ptr, ++preptr) {
		if (ptr == arr.end() || ptr->tl().y > preptr->br().y) {
			e = ptr;
			sort(s, e, cmp);
			s = ptr;
			if (ptr == arr.end())
				break;
		}
	}
}
void getSegment(const Mat& srcImage, vector<Mat>& arr, Mat& showImage) {
	Mat tmpImage = srcImage.clone();
	if (tmpImage.type() != CV_8UC1) {
		cvtColor(tmpImage, tmpImage, CV_BGR2GRAY);
	}
	threshold(tmpImage, tmpImage, 0, 255, CV_THRESH_BINARY);
	vector<vector<Point>> contours;
	findContours(tmpImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//±ßÔµ¼ì²â
	Mat tmpImage3 = srcImage.clone();
	vector<Rect> rectArr;
	for (int i = 0; i < contours.size(); ++i) {
		Rect rec = boundingRect(contours[i]);
		if (rec.area() >= 8 * 8) {
			rectArr.push_back(rec);
		}
	}
	sortRect(rectArr);
	for (int i = 0; i < rectArr.size(); ++i) {
		Mat tmp = tmpImage3(rectArr[i]);
		rectangle(showImage, rectArr[i], Scalar(100), 2);//¸ù¾Ý±ßÔµ»æÖÆ¾ØÐÎ
		normal(tmp,tmp);
		
		arr.push_back(tmp);
	}
}