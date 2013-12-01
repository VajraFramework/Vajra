#ifndef FILE_UTILITIES_H
#define FILE_UTILITIES_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <string>

void ReadTextFileTillStringToken(std::ifstream& outFile, const std::string token);

glm::mat4x4 ReadGlmMat4x4FromFile(std::ifstream& file);

#endif // FILE_UTILITIES_H
