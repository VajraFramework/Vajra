#if defined(PLATFORM_IOS)

#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Settings/SettingsHelper.h"

#include <string>

bool DoesSettingsFileExist() {
	std::string settingsPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath();
	std::string settingsFilePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
	
	NSFileManager* fileMan = [NSFileManager defaultManager];
	
	NSString* settingsPathNSString = [NSString stringWithCString:settingsPath.c_str() encoding:NSASCIIStringEncoding];
	if ([fileMan fileExistsAtPath:settingsPathNSString isDirectory:nullptr]) {
		NSString* settingsFilepathNSString = [NSString stringWithCString:(settingsFilePath.c_str()) encoding:NSASCIIStringEncoding];
		return [fileMan fileExistsAtPath:settingsFilepathNSString];
	}
	return false;
}

void GetSettingsFileHandleForReading(std::ifstream& file) {
	

	
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
	file.open(filePath);
	ASSERT(file.good(), "Valid file handle for %s", filePath.c_str());
}

void GetSettingsFileHandleForWriting(std::ofstream& file) {
	
	// Ensure that the settings file exists:
	if (!DoesSettingsFileExist()) {
		std::string settingsPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath();
		std::string settingsFilePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
		
		NSFileManager* fileMan = [NSFileManager defaultManager];
		
		NSString* settingsPathNSString = [NSString stringWithCString:settingsPath.c_str() encoding:NSASCIIStringEncoding];
		if (![fileMan fileExistsAtPath:settingsPathNSString isDirectory:nullptr]) {
			[fileMan createDirectoryAtPath:settingsPathNSString withIntermediateDirectories:NO attributes:nil error:nil];
		}
		
		NSString* settingsFilepathNSString = [NSString stringWithCString:(settingsFilePath.c_str()) encoding:NSASCIIStringEncoding];
		if (![fileMan fileExistsAtPath:settingsFilepathNSString]) {
			[fileMan createFileAtPath:settingsFilepathNSString contents:nil attributes:nil];
		}
	}
	
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSettingsResourcesPath() + SETTINGS_FILE_NAME;
	file.open(filePath);
	ASSERT(file.good(), "Valid file handle for %s", filePath.c_str());
}

#endif
