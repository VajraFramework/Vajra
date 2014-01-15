#include "Vajra/Utilities/StringUtilities.h"

int StringUtilities::ConvertStringToInt(std::string& s) {
	std::istringstream iss(s);
	int result;
	iss >> result;
	return result;
}

float StringUtilities::ConvertStringToFloat(std::string& s) {
	std::istringstream iss(s);
	float result;
	iss >> result;
	return result;
}

bool StringUtilities::ConvertStringToBool(std::string& s) {
	if (s == "false" || s == "0") {
		return false;
	}
	return true;
}
