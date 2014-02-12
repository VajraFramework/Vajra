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

	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString* documentsPathNSString = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
	std::string documentsPath = [documentsPathNSString cStringUsingEncoding:NSUTF8StringEncoding];

	this->devicePictureResourcesPath    = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_PICTURES;
	this->deviceShaderResourcesPath     = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_SHADERS;
	this->deviceModelResourcesPath      = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_MODELS;
	this->deviceArmatureResourcesPath   = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_ARMATURES;
	this->deviceAnimationResourcesPath  = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_ANIMATIONS;
	this->deviceAudioResourcesPath      = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_AUDIO;
	this->deviceLoggingResourcesPath    = documentsPath + "/"           + RESOURCES_FOLDER_NAME_LOGGING;
	this->deviceFontResourcesPath       = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_FONTS;
	this->deviceUiScenesResourcesPath   = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_UISCENES;
	this->devicePrefabsResourcesPath    = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_PREFABS;
	this->deviceTagsResourcesPath       = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_TAGS;
	this->deviceConstantsResourcesPath  = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_CONSTANTS;
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_IOS
