#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/SavedDataManager/SavedDataHelper.h"
#include "Vajra/Utilities/Utilities.h"

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_ANDROID)

void GetBundleFileHandleForReading(std::string bundleFileName, std::ifstream& file_out) {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;
	file_out.open(filePath);
	ASSERT(file_out.good(), "Valid file handle for %s", filePath.c_str());
}

void GetBundleFileHandleForWriting(std::string bundleFileName, std::ofstream& file_out) {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceSavedDataResourcesPath() + bundleFileName;
	file_out.open(filePath);
	ASSERT(file_out.good(), "Valid file handle for %s", filePath.c_str());
}

#endif
