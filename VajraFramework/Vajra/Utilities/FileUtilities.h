#ifndef FILE_UTILITIES_H
#define FILE_UTILITIES_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <string>

void ReadTextFileTillStringToken(std::istream& outFile, const std::string token);

glm::mat4x4 ReadGlmMat4x4FromFile(std::istream& file);

int ReadInt32LittleEndianFromFile(/*char buffer[5], */std::istream& file);
short ReadInt16LittleEndianFromFile(/*char buffer[5], */std::istream& file);

void AdvanceFileSeekTillChar(std::istream& file, char endChar);
std::string ReadFileTillChar(std::istream& file, char endChar, bool ignoreWhiteSpace = false);
std::string ReadFileTillBeforeChar(std::istream& file, char endChar, bool ignoreWhiteSpace = false);
void AdvanceFileSeekOverWhiteSpace(std::istream& file);
int GetNextIntFromFile(std::istream& file);

#endif // FILE_UTILITIES_H
