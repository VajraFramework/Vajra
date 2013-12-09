#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#ifdef PLATFORM_IOS

FileSystemUtils::FileSystemUtils() {
	this->init();
}

FileSystemUtils::~FileSystemUtils() {
	this->destroy();
}

void FileSystemUtils::init() {
	NSString* resourcePathNSString = [[NSBundle mainBundle] resourcePath];
	this->deviceBaseResourcesPath = [resourcePathNSString cStringUsingEncoding:NSUTF8StringEncoding];
	this->deviceBaseResourcesPath += "/";
	this->devicePictureResourcesPath    = this->deviceBaseResourcesPath + "pictures/";
	this->deviceShaderResourcesPath     = this->deviceBaseResourcesPath + "shaders/";
	this->deviceModelResourcesPath      = this->deviceBaseResourcesPath + "models/";
    this->deviceArmatureResourcesPath   = this->deviceBaseResourcesPath + "armatures/";
	this->deviceAnimationResourcesPath  = this->deviceBaseResourcesPath + "animations/";
	this->deviceAudioResourcesPath      = this->deviceBaseResourcesPath + "audio/";
	this->deviceLoggingResourcesPath    = this->deviceBaseResourcesPath + "logging/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_IOS
