#include "Vajra/Framework/Logging/Logger.h"

#ifdef PLATFORM_ANDROID
#include <android/log.h>

#define  LOG_TAG    "VAJRA"

Logger::Logger() {
}

void Logger::dbglog(const char* fmt, ...) {
	va_list args;
    va_start(args, fmt);
    this->vdbglog(fmt, args);
    va_end(args);
}

void Logger::vdbglog(const char* fmt, va_list args) {
	__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, args);
}

void Logger::errlog(const char* fmt, ...) {
	va_list args;
    va_start(args, fmt);
    this->verrlog(fmt, args);
    va_end(args);
}

void Logger::verrlog(const char* fmt, va_list args) {
	__android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, args);
}

#endif // PLATFORM_ANDROID
