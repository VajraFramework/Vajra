#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Timer/Timer.h"

Timer::Timer() {
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

	this->fiveHundredMillisecondCounter = 0.0;
	this->fiveSecondCounter = 0.0f;
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
	MessageChunk onFrameMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	onFrameMessage->SetMessageType(MESSAGE_TYPE_FRAME_EVENT);
	ENGINE->GetMessageHub()->SendMulticastMessage(onFrameMessage, this->GetId());

	this->broadcastMultiGranularMessages();
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

// Raise some multi granular update events based on frameNumber and current time
void Timer::broadcastMultiGranularMessages() {
	// Frame based messagess
	// Every 5 frames
	if(this->frameNumber % 5 == 0){
		MessageChunk onFifthFrameMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		onFifthFrameMessage->SetMessageType(MESSAGE_TYPE_FIFTH_FRAME_EVENT);
		ENGINE->GetMessageHub()->SendMulticastMessage(onFifthFrameMessage, this->GetId());

		// Every 10 frames
		if(this->frameNumber % 10 == 0){
			MessageChunk onTenthFrameMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			onTenthFrameMessage->SetMessageType(MESSAGE_TYPE_TENTH_FRAME_EVENT);
			ENGINE->GetMessageHub()->SendMulticastMessage(onTenthFrameMessage, this->GetId());
		}
	}

	// Time based messages
	// Update time counters
	double fiveHundredMillisecondCounter_temp = this->fiveHundredMillisecondCounter + this->deltaFrameTime;
	double fiveSecondCounter_temp = this->fiveSecondCounter + this->deltaFrameTime;
	// every 500 milliseconds
	if(fiveHundredMillisecondCounter_temp >= 0.5)
	{
		// Send the message
		MessageChunk every500MillisecondsMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		every500MillisecondsMessage->SetMessageType(MESSAGE_TYPE_500_MS_TIME_EVENT);
		ENGINE->GetMessageHub()->SendMulticastMessage(every500MillisecondsMessage, this->GetId());

		// Decrement counter
		fiveHundredMillisecondCounter_temp -= 0.5;

		// Every 5 seconds 
		if(fiveSecondCounter_temp >= 5.0)
		{
			// Send the message
			MessageChunk every5SecondsMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			every5SecondsMessage->SetMessageType(MESSAGE_TYPE_5_S_TIME_EVENT);
			ENGINE->GetMessageHub()->SendMulticastMessage(every5SecondsMessage, this->GetId());

			// Decrement counter
			fiveSecondCounter_temp -= 5.0;
		}
	}
	// Store temps back in counters
	this->fiveHundredMillisecondCounter = fiveHundredMillisecondCounter_temp;
	this->fiveSecondCounter = fiveSecondCounter_temp;
}

double Timer::GetHighResAbsoluteTime() {
	std::chrono::time_point<std::chrono::system_clock> clockTimeNow = std::chrono::system_clock::now();
	auto durationSinceEpoch = clockTimeNow.time_since_epoch();
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch).count() / 1000000000.0);
}

void Timer::destroy() {
}
