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

	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* documentsPathNSString = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
	std::string documentsPath = [documentsPathNSString cStringUsingEncoding:NSUTF8StringEncoding];
	
	this->deviceLoggingResourcesPath    = documentsPath + "/logging/";
	this->deviceFontResourcesPath       = this->deviceBaseResourcesPath + "fonts/";
	this->deviceUiScenesResourcesPath   = this->deviceBaseResourcesPath + "uiscenes/";
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_IOS
