#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"
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

	ENGINE->GetLogger()->dbglog("\nStackTrace: \n");
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

		ENGINE->GetLogger()->dbglog("\n! FAILED ASSERT: ");
		ENGINE->GetLogger()->dbglog("\nIn file %s:%d: In function \'%s()\'\n    Failed while asserting that >> \"", file, lineNumber, functionName);
		ENGINE->GetLogger()->vdbglog(fmt, args);
		ENGINE->GetLogger()->dbglog("\"\n");

		printStackTrace();
		va_end(args);

		assert(0);
	}
}

void ASSERT_LOG_internal(const char* file, int lineNumber, const char* functionName, bool expression, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	if (!expression) {
		ENGINE->GetLogger()->dbglog("\n! FAILED ASSERT: ");
		ENGINE->GetLogger()->dbglog("\nIn file %s:%d: In function \'%s()\'\n    Failed while asserting that >> \"", file, lineNumber, functionName);
		ENGINE->GetLogger()->vdbglog(fmt, args);
		ENGINE->GetLogger()->dbglog("\"\n");

		printStackTrace();
		assert(0);

	} else {
		ENGINE->GetLogger()->dbglog("\nAssert true: ");
		ENGINE->GetLogger()->vdbglog(fmt, args);
		ENGINE->GetLogger()->dbglog("\n\tIn file %s:%d: In function \'%s()\': ", file, lineNumber, functionName);
	}

	va_end(args);
}


void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    ENGINE->GetLogger()->dbglog("GL %s = %s\n", name, v);
}

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        ENGINE->GetLogger()->errlog("\nGLERROR: after %s() glError (0x%x)\n", op, error);
    }
}
