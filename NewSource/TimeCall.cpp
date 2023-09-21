#include "TimeCall.h"

TimeCall::TimeCall(std::function<void()> setFunction, uint32_t time) 
	: CallBackFunction_(setFunction), time_(time) {}

void TimeCall::Update() {

	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		CallBackFunction_();
	}

}