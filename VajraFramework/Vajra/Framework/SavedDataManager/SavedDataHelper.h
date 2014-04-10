#ifndef SAVED_DATA_HELPER_H
#define SAVED_DATA_HELPER_H

#include <string>
#include <fstream>

#ifdef PLATFORM_IOS
bool DoesBundleFileExist(std::string bundleFileName);
#endif

void GetBundleFileHandleForReading(std::string bundleFileName, std::ifstream& file_out);
void GetBundleFileHandleForWriting(std::string bundleFileName, std::ofstream& file_out);

#endif // SAVED_DATA_HELPER_H
