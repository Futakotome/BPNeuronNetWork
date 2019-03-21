#pragma once
#include<queue>
#include<thread>
#include<string>
#include<mutex>

class ProgressBar
{
public:
	int max;
	int curProgress = 0;
	int currentValue = 0;
	std::queue<int> upgradeQueue;
	bool stopThread = false;
	static void* progressThread(void *__this);
	std::thread * printThread{};
	std::mutex mtx;
public:

	void updateValue(int value);
	ProgressBar(int max, const std::string& label);
	explicit ProgressBar(int max);
	ProgressBar& operator++();
	ProgressBar& operator+=(int max);
	static void show();
	void shutDown();
	void reset(int max, const std::string& label);

	~ProgressBar();


};