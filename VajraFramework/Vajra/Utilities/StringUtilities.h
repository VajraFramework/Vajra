#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <string>
#include <sstream>
#include <vector>

namespace StringUtilities {
	int   ConvertStringToInt  (std::string& s);
	float ConvertStringToFloat(std::string& s);
	bool  ConvertStringToBool (std::string& s);

	std::vector<std::string> SplitStringIntoTokensOnDelimiter (std::string& s, char delimiter, bool removeWhiteSpace = false);

	std::string EraseStringFromString(std::string bigString, std::string smallString, bool onlyFirstOccurance = false);

	bool FindStringInVectorOfStrings(std::vector<std::string> v, std::string stringToFind);
}
#define ConvertStringToString(s) s

#endif // STRING_UTILITIES_H
