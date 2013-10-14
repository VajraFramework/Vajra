#ifndef TIMER_H
#define TIMER_H

#include "Vajra/Common/Objects/Object.h"

#include <chrono>

class Timer : public Object {
public:
	~Timer();

	inline unsigned long long GetFrameNumber() { return this->frameNumber; }
	double GetDeltaFrameTime() { return this->deltaFrameTime; }
	double GetFPS() { return this->fps; }

	inline unsigned long long GetSecondsSinceBoot() { return this->secondsSinceBoot; }
	inline unsigned long long GetSecondsSinceEpoch() { return this->secondsSinceEpoch; }

	// Potentially expensive; Use only if necessary
	inline double GetTimeSinceFrameBegin();

private:
	Timer();
	void init();
	void destroy();

	void update();

	unsigned long long frameNumber;
	unsigned long long secondsSinceEpoch;
	unsigned long long secondsSinceBoot;

	std::chrono::time_point<std::chrono::system_clock> frameSystemClock;

	double fps;
	double deltaFrameTime;

	unsigned long long secondsSinceEpochAtBoot;

	friend class Engine;
};

// Inline Functions:

inline double Timer::GetTimeSinceFrameBegin() {
	// TODO [Implement]
	return 0.0;
}

#endif // TIMER_H
