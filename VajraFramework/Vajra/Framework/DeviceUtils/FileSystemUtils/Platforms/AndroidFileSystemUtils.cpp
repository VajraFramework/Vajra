#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/VajraToAndroidJavaInterface/VajraToAndroidJavaInterface.h"

#ifdef PLATFORM_ANDROID

FileSystemUtils::FileSystemUtils() {
    this->init();
}

FileSystemUtils::~FileSystemUtils() {
    this->destroy();
}

void FileSystemUtils::init() {
    this->deviceBaseResourcesPath = GetAndroidDeviceBaseResourcesPath();
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
