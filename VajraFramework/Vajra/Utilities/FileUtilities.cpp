#include "Vajra/Utilities/FileUtilities.h"

void ReadTextFileTillStringToken(std::ifstream& inFile, const std::string token) {
	std::string tempBuffer;
	while (inFile.good()) {
		inFile >>tempBuffer;
		if (tempBuffer == token) {
			break;
		}
	}
}

glm::mat4x4 ReadGlmMat4x4FromFile(std::ifstream& file) {
	glm::mat4x4 m;
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			file >> m[r][c];
		}
	}
	return m;
}

int ReadInt32LittleEndianFromFile(/*char buffer[5], */std::ifstream& file) {
	int val = 0;
	unsigned char buffer[5];
	file.read((char*)buffer, 4);
	val = (buffer[0]) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
	return val;
}

short ReadInt16LittleEndianFromFile(/*char buffer[5], */std::ifstream& file) {
	short val = 0;
	unsigned char buffer[3];
	file.read((char*)buffer, 2);
	val = (buffer[0]) | (buffer[1] << 8);
	return val;
}

void AdvanceFileSeekTillChar(std::ifstream& file, char endChar) {
	char c;
	do {
		file >> c;
	} while (c != endChar && !file.eof ());

	return;
}

std::string ReadFileTillChar(std::ifstream& file, char endChar, bool ignoreWhiteSpace /* = false */) {
	char c;
	std::string returnString;
	do {
		file.get(c);
		if (c != endChar) {
			if (!ignoreWhiteSpace || (c != ' ' && c != '\t')) {
				returnString.push_back(c);
			}
		}
	} while (c != endChar && !file.eof ());

	return returnString;
}

std::string ReadFileTillBeforeChar(std::ifstream& file, char endChar, bool ignoreWhiteSpace) {
	if (!file.good()) {
		return "";
	}

	char c;
	std::string returnString;
	do {
		c = file.peek();
		if (c != endChar) {
			file.get(c);
			if (!ignoreWhiteSpace || (c != ' ' && c != '\t')) {
				returnString.push_back(c);
			}
		}
		if (c == endChar || !file.good()) {
			return returnString;
		}
	} while (true);
}

void AdvanceFileSeekOverWhiteSpace(std::ifstream& file) {
	if (!file.good()) {
		return;
	}

	char c;
	do {
		c = file.peek();
		if (c == ' ' || c == '\t' || c == '\n') {
			file.get(c);
		}
	} while ((c == ' ' || c == '\t' || c == '\n') && !file.eof());
}

int GetNextIntFromFile(std::ifstream& file) {
	int r;
	file >> r;

	return r;
}
