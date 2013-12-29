#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#ifdef PLATFORM_ANDROID

FileSystemUtils::FileSystemUtils() {
    this->init();
}

FileSystemUtils::~FileSystemUtils() {
    this->destroy();
}

void FileSystemUtils::init() {
    // TODO [Implement] Implement proper deviceResourcesPath on Android:
    this->deviceBaseResourcesPath = "/sdcard/download/";
    this->devicePictureResourcesPath    = this->deviceBaseResourcesPath + "pictures/";
    this->deviceShaderResourcesPath     = this->deviceBaseResourcesPath + "shaders/";
    this->deviceModelResourcesPath      = this->deviceBaseResourcesPath + "models/";
    this->deviceArmatureResourcesPath   = this->deviceBaseResourcesPath + "armatures/";
    this->deviceAnimationResourcesPath  = this->deviceBaseResourcesPath + "animations/";
	this->deviceAudioResourcesPath      = this->deviceBaseResourcesPath + "audio/";
	this->deviceLoggingResourcesPath    = this->deviceBaseResourcesPath + "logging/";
	this->deviceFontResourcesPath       = this->deviceBaseResourcesPath + "fonts/";
	this->deviceUiScenesResourcesPath   = this->deviceBaseResourcesPath + "uiscenes/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_ANDROID
