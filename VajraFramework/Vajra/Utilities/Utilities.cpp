#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLCounter/OpenGLCounter.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Utilities/Utilities.h"

#include <assert.h>
#include <cstdarg>


#if defined(PLATFORM_DESKTOP) && defined(PLATFORM_LINUX)
#include <execinfo.h>
#include <unistd.h>
void printStackTrace() {
	void *array[10];
	size_t size;

	size = backtrace(array, 10);

	FRAMEWORK->GetLogger()->dbglog("\nStackTrace: \n");
	backtrace_symbols_fd(array, size, STDOUT_FILENO);
}
#else
void printStackTrace() {
	// TODO [Implement] printStackTrace() for non-linux platforms
}
#endif // PLATFORM_DESKTOP && PLATFORM_LINUX

void ASSERT_internal(const char* file, int lineNumber, const char* functionName, bool expression, const char* fmt, ...) {
	if (!expression) {
		va_list args;
		va_start(args, fmt);

		FRAMEWORK->GetLogger()->dbglog("\n! FAILED ASSERT: ");
		FRAMEWORK->GetLogger()->dbglog("\nIn file %s:%d: In function \'%s()\'\n    Failed while asserting that >> \"", file, lineNumber, functionName);
		FRAMEWORK->GetLogger()->vdbglog(fmt, args);
		FRAMEWORK->GetLogger()->dbglog("\"\n");

		printStackTrace();
		va_end(args);

		assert(0);
	}
}

void ASSERT_LOG_internal(const char* file, int lineNumber, const char* functionName, bool expression, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	if (!expression) {
		FRAMEWORK->GetLogger()->dbglog("\n! FAILED ASSERT: ");
		FRAMEWORK->GetLogger()->dbglog("\nIn file %s:%d: In function \'%s()\'\n    Failed while asserting that >> \"", file, lineNumber, functionName);
		FRAMEWORK->GetLogger()->vdbglog(fmt, args);
		FRAMEWORK->GetLogger()->dbglog("\"\n");

		printStackTrace();
		assert(0);

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nAssert true: ");
		FRAMEWORK->GetLogger()->vdbglog(fmt, args);
		FRAMEWORK->GetLogger()->dbglog("\n\tIn file %s:%d: In function \'%s()\': ", file, lineNumber, functionName);
	}

	va_end(args);
}


void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);		// GLCALL
    FRAMEWORK->GetLogger()->dbglog("GL %s = %s\n", name, v);
}

void checkGlError(const char* op) {
#ifdef DEBUG
    for (GLint error = glGetError(); error; error = glGetError()) {
        FRAMEWORK->GetLogger()->errlog("\nGLERROR: after %s() glError (0x%x)\n", op, error);
    }
#endif // DEBUG
}

void countGlCall(const char* glFunctionName) {
	FRAMEWORK->GetOpenGLWrapper()->GetOpenGLCounter()->CountGlCall(glFunctionName);
}

void printFrameTimeStats() {
	double deltaFramesTime = ENGINE->GetTimer()->GetDeltaFrameTime();
	double frameDuration = ENGINE->GetTimer()->GetTotalFrameTime();
	double renderDuration = ENGINE->GetTimer()->GetRenderPhaseTime();
	double updateDuration = ENGINE->GetTimer()->GetUpdatePhaseTime();
	double inputDuration = ENGINE->GetTimer()->GetInputPhaseTime();

	FRAMEWORK->GetLogger()->dbglog("\nframe: %llu, fps: %f, delta: %f ms, ssb: %llu",
			ENGINE->GetTimer()->GetFrameNumber(),
			ENGINE->GetTimer()->GetFPS(),
			deltaFramesTime * 1000.0,
			ENGINE->GetTimer()->GetSecondsSinceBoot());

	FRAMEWORK->GetLogger()->dbglog("\nFrame Time Split: total: %f ms (%f \%), input: %f ms (%f \%), update: %f ms (%f \%), render: %f ms (%f \%)",
			frameDuration * 1000.0, frameDuration/deltaFramesTime * 100.0,
			inputDuration * 1000.0, inputDuration/deltaFramesTime * 100.0,
			updateDuration * 1000.0, updateDuration/deltaFramesTime * 100.0,
			renderDuration * 1000.0, renderDuration/deltaFramesTime * 100.0);
}

void printGlmMat4(glm::mat4 m) {
	FRAMEWORK->GetLogger()->dbglog("\n");
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			FRAMEWORK->GetLogger()->dbglog("%f ", m[r][c]);
		}
		FRAMEWORK->GetLogger()->dbglog("\n");
	}
}
