#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include <sstream>

namespace StringUtilities {
	int   ConvertStringToInt  (std::string& s);
	float ConvertStringToFloat(std::string& s);
	bool  ConvertStringToBool (std::string& s);
}
#define ConvertStringToString(s) s

#endif // STRING_UTILITIES_H
