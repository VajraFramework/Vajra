#if defined(PLATFORM_IOS)

#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/SavedDataManager/SavedDataHelper.h"
#include "Vajra/Utilities/Utilities.h"

#include <string>

bool DoesBundleFileExist(std::string bundleFileName) {
	std::string savedDataPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath();
	std::string savedDataFilePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;

	NSFileManager* fileMan = [NSFileManager defaultManager];
		
	NSString* savedDataPathNSString = [NSString stringWithCString:savedDataPath.c_str() encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataPathNSString isDirectory:nullptr]) {
		[fileMan createDirectoryAtPath:savedDataPathNSString withIntermediateDirectories:NO attributes:nil error:nil];
		return false;
	}
		
	NSString* savedDataFilepathNSString = [NSString stringWithCString:(savedDataFilePath.c_str()) encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataFilepathNSString]) {
		return false;
	}
	return true;
}

void GetBundleFileHandleForReading(std::string bundleFileName, std::ifstream& file_out) {

	std::string savedDataPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath();
	std::string savedDataFilePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;

	NSFileManager* fileMan = [NSFileManager defaultManager];
		
	NSString* savedDataPathNSString = [NSString stringWithCString:savedDataPath.c_str() encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataPathNSString isDirectory:nullptr]) {
		[fileMan createDirectoryAtPath:savedDataPathNSString withIntermediateDirectories:NO attributes:nil error:nil];
	}
		
	NSString* savedDataFilepathNSString = [NSString stringWithCString:(savedDataFilePath.c_str()) encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataFilepathNSString]) {
		[fileMan createFileAtPath:savedDataFilepathNSString contents:nil attributes:nil];
	}

	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;
	file_out.open(filePath);
	ASSERT(file_out.good(), "Valid file handle for %s", filePath.c_str());
}

void GetBundleFileHandleForWriting(std::string bundleFileName, std::ofstream& file_out) {
	std::string savedDataPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath();
	std::string savedDataFilePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;

	NSFileManager* fileMan = [NSFileManager defaultManager];
		
	NSString* savedDataPathNSString = [NSString stringWithCString:savedDataPath.c_str() encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataPathNSString isDirectory:nullptr]) {
		[fileMan createDirectoryAtPath:savedDataPathNSString withIntermediateDirectories:NO attributes:nil error:nil];
	}
		
	NSString* savedDataFilepathNSString = [NSString stringWithCString:(savedDataFilePath.c_str()) encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:savedDataFilepathNSString]) {
		[fileMan createFileAtPath:savedDataFilepathNSString contents:nil attributes:nil];
	}

	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;
	file_out.open(filePath);
	ASSERT(file_out.good(), "Valid file handle for %s", filePath.c_str());
}

#endif

