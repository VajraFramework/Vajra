#include "Vajra/Engine/Timer/Timer.h"

Timer::Timer() {
	this->init();
}

Timer::~Timer() {
	this->destroy();
}

void Timer::init() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto durationSinceEpoch = clockTimeNow.time_since_epoch();

	this->frameSystemClock = clockTimeNow;

	this->secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();
	this->secondsSinceEpochAtBoot = this->secondsSinceEpoch;

	this->frameNumber = 0;
	this->secondsSinceBoot = 0;

	// Initialize with fake values; will correct itself after 1 frame
	double FAKE_FPS = 30.0;
	this->fps = FAKE_FPS;
	this->deltaFrameTime = 1.0 / FAKE_FPS;
}

void Timer::update() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto durationSincePrevFrame = clockTimeNow - this->frameSystemClock;
	auto durationSinceEpoch = clockTimeNow.time_since_epoch();
	double highResSecondsSincePrevFrame = (std::chrono::duration_cast<std::chrono::nanoseconds>(durationSincePrevFrame).count() / 1000000000.0);
	unsigned long long lowResSecondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();

	// Update high res time fields
	this->frameSystemClock = clockTimeNow;
	this->deltaFrameTime = highResSecondsSincePrevFrame;
	this->fps = 1.0 / highResSecondsSincePrevFrame;

	// Update low res time fields
	this->secondsSinceEpoch = lowResSecondsSinceEpoch;
	this->secondsSinceBoot = lowResSecondsSinceEpoch - this->secondsSinceEpochAtBoot;

	this->frameNumber++;
}

void Timer::destroy() {
}
