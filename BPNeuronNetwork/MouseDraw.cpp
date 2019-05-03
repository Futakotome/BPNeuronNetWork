#include"pch.h"
#include"MouseDraw.h"
BoxExtractor::BoxExtractor() {

}
void BoxExtractor::mouseHandler(int event, int x, int y, int flags, void *param) {
	BoxExtractor *self = static_cast<BoxExtractor*>(param);
	self->opencv_mouse_callback(event, x, y, flags, param);
}
void BoxExtractor::opencv_mouse_callback(int event, int x, int y, int, void *param) {
	handlerT * data = (handlerT *)param;
	switch (event)
	{
	case EVENT_MOUSEMOVE:
		if (data->isDrawing) {
			data->points.push_back(cv::Point(x, y));
		}
		break;
	case EVENT_LBUTTONDOWN:
		data->isDrawing = true;
		data->points.clear();
		data->points.push_back(cv::Point(x, y));
		break;
	case EVENT_LBUTTONUP:
		data->isDrawing = false;
		break;
	}
}
Mat BoxExtractor::MouseDraw(Mat& img) {
	MouseDraw("Draw an digital", img, Scalar(0, 0, 0), 4);
	return img;
}
int BoxExtractor::MouseDraw(const std::string& windowName, Mat& img, Scalar color, int border) {
	int key = 0;
	imshow(windowName, img);
	cout << "利用鼠标写下一个或多个数字，按回车输出预测结果并进行下一次书写，esc退出" << endl;
	params.image = img.clone();
	setMouseCallback(windowName, mouseHandler, (void *)&params);
	int cnt = 0;
	while (!(key == 32 || key == 27 || key == 13)) {
		int length = params.points.size();
		for (int i = 0; i < length&&length>2; i++) {
			if (i == length - 1) {
				cv::line(params.image, params.points.at(i), params.points.at(i), color, border);
			}
			else {
				cv::line(params.image, params.points.at(i), params.points.at(i + 1), color, border);
			}
		}
		imshow(windowName, params.image);
		key = waitKey(1);

	}
	params.image.copyTo(img);
	return key;
}