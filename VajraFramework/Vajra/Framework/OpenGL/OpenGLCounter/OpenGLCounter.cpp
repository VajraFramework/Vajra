#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLCounter/OpenGLCounter.h"

OpenGLCounter::OpenGLCounter() {
	this->init();
}

OpenGLCounter::~OpenGLCounter() {
	this->destroy();
}

void OpenGLCounter::CountGlCall(const char* /* glFunctionName */) {
	this->countOfAllGlCallsThisFrame++;
}

void OpenGLCounter::switchFrames() {
	// FRAMEWORK->GetLogger()->dbglog("\nNumber of gl calls in previous frame: %u", this->countOfAllGlCallsThisFrame);
	this->countOfAllGlCallsThisFrame = 0;
}

void OpenGLCounter::init() {
	this->countOfAllGlCallsThisFrame = 0;
}

void OpenGLCounter::destroy() {
}
