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
