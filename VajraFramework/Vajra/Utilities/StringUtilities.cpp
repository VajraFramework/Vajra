#include "Vajra/Utilities/StringUtilities.h"


#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <locale>

int StringUtilities::ConvertStringToInt(std::string s) {
	std::istringstream iss(s);
	int result;
	iss >> result;
	return result;
}

float StringUtilities::ConvertStringToFloat(std::string s) {
	std::istringstream iss(s);
	float result;
	iss >> result;
	return result;
}

bool StringUtilities::ConvertStringToBool(std::string s) {
	if (StringUtilities::StringToLower(s) == "false" || s == "0") {
		return false;
	}
	return true;
}

std::string StringUtilities::ConvertIntToString(int number) {
	char buffer[20];
	sprintf(buffer, "%d", number);
	std::string resultString = buffer;
	return resultString;
}

std::vector<std::string> StringUtilities::SplitStringIntoTokensOnDelimiter(std::string& s, char delimiter, bool removeWhiteSpace /* = false */) {
	std::vector<std::string> tokens;
	std::istringstream iss(s);
	std::string token;
	//
	while (std::getline(iss, token, delimiter)) {
		if (token.size() > 0 && token[0] != delimiter) {
			if (removeWhiteSpace) {
				token = StringUtilities::EraseStringFromString(token, " ");
			}
			tokens.push_back(token);
		}
	}

	return tokens;
}

std::string StringUtilities::EraseStringFromString(std::string bigString, std::string smallString, bool onlyFirstOccurance /* = false */) {
	if (bigString.find(smallString) == std::string::npos) {
		return bigString;
	}
	do {
		bigString.erase(bigString.find(smallString), smallString.size());
	} while (!onlyFirstOccurance && bigString.find(smallString) != std::string::npos);

	return bigString;
}

bool StringUtilities::FindStringInVectorOfStrings(std::vector<std::string> v, std::string stringToFind) {
	for (std::string s : v) {
		if (s == stringToFind) {
			return true;
		}
	}
	return false;
}

std::string StringUtilities::StringToLower(std::string s) {
	std::string result;
	for (char c : s) {
		result.push_back(std::tolower(c));
	}
	return result;
}

std::string StringUtilities::StringToUpper(std::string s) {
	std::string result;
	for (char c : s) {
		result.push_back(std::toupper(c));
	}
	return result;
}
