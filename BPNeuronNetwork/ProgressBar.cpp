#include"pch.h"
#include"ProgressBar.h"
#include<iostream>
#include<iomanip>

using namespace std;

void* ProgressBar::progressThread(void *__this) {
	ProgressBar * _this = static_cast<ProgressBar *>(__this);
	while (!_this->stopThread) {
		if (!_this->upgradeQueue.empty()) {
			if (_this->mtx.try_lock()) {
				const int value = _this->upgradeQueue.front();
				_this->upgradeQueue.pop();
				_this->updateValue(value);
				_this->mtx.unlock();
			}
		}
	}
	return nullptr;
}
ProgressBar::ProgressBar(const int max) :max(max), printThread(new thread(&progressThread, static_cast<void *>(this))) {

}
ProgressBar::~ProgressBar() {
	stopThread = true;
	if (printThread->joinable())
		printThread->join();
	delete printThread;
}
ProgressBar& ProgressBar::operator++() {
	if (mtx.try_lock()) {
		upgradeQueue.push(1);
		mtx.unlock();
	}
	return *this;
}
ProgressBar& ProgressBar::operator+=(int value) {
	if (mtx.try_lock()) {
		upgradeQueue.push(value);
		mtx.unlock();
	}
	return *this;
}
void ProgressBar::show() {
	cout << "0%  10%  20%  30%  40%  50%  60%  70%  80%  90%  100%" << endl;
	cout << "|----|----|----|----|----|----|----|----|----|----|" << endl;
}
void ProgressBar::shutDown() {
	stopThread = true;

}
void ProgressBar::reset(int max, const std::string& label) {
	this->max = max;
	cout << endl << label << endl;
	show();

	curProgress = 0;
	currentValue = 0;
	while (!upgradeQueue.empty())
	{
		upgradeQueue.pop();
	}
}
inline void ProgressBar::updateValue(const int value) {
	currentValue += value;
	const int prog = static_cast<float>(currentValue) * 50 / max;
	if (prog > curProgress) {
		cout << "\r" << setfill('*') << setw(prog) << "*";
		curProgress = prog;
		if (curProgress >= 50) {
			cout << "*" << endl;
		}
	}
}
ProgressBar::ProgressBar(int max, const std::string& label) :ProgressBar(max) {
	cout << label << endl;
	show();
}