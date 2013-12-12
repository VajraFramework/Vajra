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
	double GetHighResAbsoluteTime();

	// Potentially expensive; Use only if necessary
	inline double GetTimeSinceFrameBegin();

	inline double GetTotalFrameTime()  { return this->totalFrameDuration; }
	inline double GetRenderPhaseTime() { return this->renderPhaseDuration; }
	inline double GetUpdatePhaseTime() { return this->updatePhaseDuration; }
	inline double GetInputPhaseTime()  { return this->inputPhaseDuration; }

private:
	Timer();
	void init();
	void destroy();

	void beginFrame();
	void beginRenderPhase();
	void endRenderPhase();
	void beginUpdatePhase();
	void endUpdatePhase();
	void beginInputPhase();
	void endInputPhase();
	void endFrame();
	void broadcastMultiGranularMessages();

	unsigned long long frameNumber;
	unsigned long long secondsSinceEpoch;
	unsigned long long secondsSinceBoot;

	std::chrono::time_point<std::chrono::system_clock> frameBeginSystemClock;
	std::chrono::time_point<std::chrono::system_clock> renderPhaseBeginSystemClock;
	std::chrono::time_point<std::chrono::system_clock> updatePhaseBeginSystemClock;
	std::chrono::time_point<std::chrono::system_clock> inputPhaseBeginSystemClock;

	double fps;
	double deltaFrameTime;

	double totalFrameDuration;
	double renderPhaseDuration;
	double updatePhaseDuration;
	double inputPhaseDuration;

	// Multi Granular counters
	double fiveHundredMillisecondCounter;
	double fiveSecondCounter;

	unsigned long long secondsSinceEpochAtBoot;

	friend class Engine;
};

// Inline Functions:

inline double Timer::GetTimeSinceFrameBegin() {
	// TODO [Implement]
	return 0.0;
}

#endif // TIMER_H
