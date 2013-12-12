#ifndef UTILITIES_H
#define UTILITIES_H

#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"

#include <string>
#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define ASSERT(x, fmt, ...) \
	ASSERT_internal(__FILE__, __LINE__, __FUNCTION__,	\
					x, fmt , ##__VA_ARGS__)

#define ASSERT_LOG(x, fmt, ...) \
	ASSERT_LOG_internal(__FILE__, __LINE__, __FUNCTION__,	\
					x, fmt , ##__VA_ARGS__)
#else
////////////////////////////////////////////////////////////////////////////////
// Asserts are compiled out in non-debug builds:
#define ASSERT(x, fmt, ...)
#define ASSERT_LOG(x, fmt, ...)
#endif // DEBUG
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#define VERIFY(x, fmt, ...) \
	ASSERT_internal(__FILE__, __LINE__, __FUNCTION__,	\
					x, fmt , ##__VA_ARGS__)

#define VERIFY_LOG(x, fmt, ...) \
	ASSERT_LOG_internal(__FILE__, __LINE__, __FUNCTION__,	\
					x, fmt , ##__VA_ARGS__)
////////////////////////////////////////////////////////////////////////////////
	

////////////////////////////////////////////////////////////////////////////////
void ASSERT_internal(const char* file, int lineNumber, const char* functionName, bool expression, const char* fmt, ...);
void ASSERT_LOG_internal(const char* file, int lineNumber, const char* functionName, bool expression, const char* fmt, ...);
////////////////////////////////////////////////////////////////////////////////


void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);

void printFrameTimeStats();

void printGlmMat4(glm::mat4 m);

#define TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(TemplateName, returnType, baseClassType)	\
template <typename TemplateName>																\
typename std::enable_if<std::is_base_of<baseClassType, TemplateName>::value, returnType>::type

typedef int ComponentIdType;

#endif // UTILITIES_H
