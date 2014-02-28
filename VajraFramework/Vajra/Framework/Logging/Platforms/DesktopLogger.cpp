#include "Vajra/Framework/Logging/Logger.h"

#ifdef PLATFORM_DESKTOP
#include <cstdio>

Logger::Logger() {
}

void Logger::dbglog(const char* fmt, ...) {
	va_list args;
    va_start(args, fmt);
    this->vdbglog(fmt, args);
    va_end(args);
    fflush(stdout);
}

void Logger::vdbglog(const char* fmt, va_list args) {
    vprintf(fmt, args);
}

void Logger::errlog(const char* fmt, ...) {
	va_list args;
    va_start(args, fmt);
    this->verrlog(fmt, args);
    va_end(args);
}

void Logger::verrlog(const char* fmt, va_list args) {
    vprintf(fmt, args);
}

#endif // PLATFORM_DESKTOP
