#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
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

	this->frameBeginSystemClock = clockTimeNow;
	this->renderPhaseBeginSystemClock = clockTimeNow;
	this->updatePhaseBeginSystemClock = clockTimeNow;
	this->inputPhaseBeginSystemClock = clockTimeNow;

	this->secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();
	this->secondsSinceEpochAtBoot = this->secondsSinceEpoch;

	this->frameNumber = 0;
	this->secondsSinceBoot = 0;

	// Initialize with fake values; will correct itself after 1 frame
	double FAKE_FPS = 30.0;
	this->fps = FAKE_FPS;
	this->deltaFrameTime = 1.0 / FAKE_FPS;

	this->totalFrameDuration  = 0.0;
	this->renderPhaseDuration = 0.0;
	this->updatePhaseDuration = 0.0;
	this->inputPhaseDuration  = 0.0;
}

void Timer::beginFrame() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto durationSincePrevFrame = clockTimeNow - this->frameBeginSystemClock;
	auto durationSinceEpoch = clockTimeNow.time_since_epoch();
	double highResSecondsSincePrevFrame = (std::chrono::duration_cast<std::chrono::nanoseconds>(durationSincePrevFrame).count() / 1000000000.0);
	unsigned long long lowResSecondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();

	// Update high res time fields
	this->frameBeginSystemClock = clockTimeNow;
	this->deltaFrameTime = highResSecondsSincePrevFrame;
	this->fps = 1.0 / highResSecondsSincePrevFrame;

	// Update low res time fields
	this->secondsSinceEpoch = lowResSecondsSinceEpoch;
	this->secondsSinceBoot = lowResSecondsSinceEpoch - this->secondsSinceEpochAtBoot;

	this->frameNumber++;

	// Raise the onFrame event for all interested subscribers:
	const Message* const onFrameMessage = new Message(MESSAGE_TYPE_FRAME_EVENT);
	ENGINE->GetMessageHub()->SendMulticastMessage(onFrameMessage, this->GetId());
	delete onFrameMessage;
}

void Timer::beginRenderPhase() {
	this->renderPhaseBeginSystemClock = std::chrono::system_clock::now();
}

void Timer::endRenderPhase() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto duration = clockTimeNow - this->renderPhaseBeginSystemClock;
	this->renderPhaseDuration = (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);
}

void Timer::beginUpdatePhase() {
	this->updatePhaseBeginSystemClock = std::chrono::system_clock::now();
}

void Timer::endUpdatePhase() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto duration = clockTimeNow - this->updatePhaseBeginSystemClock;
	this->updatePhaseDuration = (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);
}

void Timer::beginInputPhase() {
	this->inputPhaseBeginSystemClock = std::chrono::system_clock::now();
}

void Timer::endInputPhase() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto duration = clockTimeNow - this->inputPhaseBeginSystemClock;
	this->inputPhaseDuration = (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);
}

void Timer::endFrame() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto duration = clockTimeNow - this->frameBeginSystemClock;
	this->totalFrameDuration = (std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 1000000000.0);
}

double Timer::GetHighResAbsoluteTime() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto durationSinceEpoch = clockTimeNow.time_since_epoch();
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch).count() / 1000000000.0);
}

void Timer::destroy() {
}
