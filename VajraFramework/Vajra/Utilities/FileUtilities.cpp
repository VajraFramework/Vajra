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
