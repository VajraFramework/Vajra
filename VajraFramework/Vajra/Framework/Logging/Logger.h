#ifndef LOGGER_H
#define LOGGER_H

#define  LOG_TAG    "VAJRA"

#include <cstdarg>
/*

 * This class is implemented by each of the different platforms
 * in whatever way is appropriate for that platform.
 */
class Logger {
public:
	void dbglog(const char* fmt, ...);
	void vdbglog(const char* fmt, va_list args);

	void errlog(const char* fmt, ...);
	void verrlog(const char* fmt, va_list args);

private:
	Logger();

	friend class Engine;
};

#endif // LOGGER_H
