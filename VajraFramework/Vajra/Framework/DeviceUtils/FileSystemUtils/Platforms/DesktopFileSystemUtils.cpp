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
	this->devicePictureResourcesPath    = this->deviceBaseResourcesPath + "pictures/";
	this->deviceShaderResourcesPath     = this->deviceBaseResourcesPath + "shaders/";
	this->deviceModelResourcesPath      = this->deviceBaseResourcesPath + "models/";
	this->deviceArmatureResourcesPath   = this->deviceBaseResourcesPath + "armatures/";
	this->deviceAnimationResourcesPath  = this->deviceBaseResourcesPath + "animations/";
	this->deviceAudioResourcesPath      = this->deviceBaseResourcesPath + "audio/";
	this->deviceLoggingResourcesPath    = this->deviceBaseResourcesPath + "logging/";
	this->deviceFontResourcesPath       = this->deviceBaseResourcesPath + "fonts/";
	this->deviceUiScenesResourcesPath   = this->deviceBaseResourcesPath + "uiscenes/";
	this->devicePrefabsResourcesPath    = this->deviceBaseResourcesPath + "prefabs/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_DESKTOP
