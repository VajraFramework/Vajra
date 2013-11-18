#ifndef FILE_UTILITIES_H
#define FILE_UTILITIES_H

#include <fstream>
#include <string>

void ReadTextFileTillStringToken(std::ifstream& outFile, const std::string token);

#endif // FILE_UTILITIES_H
