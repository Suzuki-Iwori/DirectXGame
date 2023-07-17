#pragma once

#include <functional>

class TimeCall {
public:
	TimeCall(std::function<void()> setFunction, uint32_t time);

	void Update();

	bool IsFinish() { return isFinish_; }

private:
	std::function<void()> CallBackFunction_;
	uint32_t time_;
	bool isFinish_ = false;
};
