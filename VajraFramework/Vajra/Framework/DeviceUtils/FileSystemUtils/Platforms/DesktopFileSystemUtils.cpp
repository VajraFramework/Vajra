#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#ifdef PLATFORM_DESKTOP

FileSystemUtils::FileSystemUtils() {
    this->init();
}

FileSystemUtils::~FileSystemUtils() {
    this->destroy();
}

void FileSystemUtils::init() {
    this->deviceBaseResourcesPath = "./Resources/";
    this->devicePictureResourcesPath = this->deviceBaseResourcesPath + "pictures/";
    this->deviceShaderResourcesPath  = this->deviceBaseResourcesPath + "shaders/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_DESKTOP
