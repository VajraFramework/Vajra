// #if defined(PLATFORM_DESKTOP)

#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Settings/SettingsHelper.h"

#include <string>

void GetSettingsFileHandleForReading(std::ifstream& file) {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
	file.open(filePath);
	ASSERT(file.good(), "Valid file handle for %s", filePath.c_str());
}

void GetSettingsFileHandleForWriting(std::ofstream& file) {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
	file.open(filePath);
	ASSERT(file.good(), "Valid file handle for %s", filePath.c_str());
}
