#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#ifdef PLATFORM_DESKTOP

FileSystemUtils::FileSystemUtils() {
    this->init();
}

FileSystemUtils::~FileSystemUtils() {
    this->destroy();
}

void FileSystemUtils::init() {
	this->deviceBaseResourcesPath = "./GameResources/";
	this->devicePictureResourcesPath    = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_PICTURES;
	this->deviceShaderResourcesPath     = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_SHADERS;
	this->deviceModelResourcesPath      = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_MODELS;
	this->deviceArmatureResourcesPath   = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_ARMATURES;
	this->deviceAnimationResourcesPath  = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_ANIMATIONS;
	this->deviceAudioResourcesPath      = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_AUDIO;
	this->deviceLoggingResourcesPath    = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_LOGGING;
	this->deviceFontResourcesPath       = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_FONTS;
	this->deviceUiScenesResourcesPath   = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_UISCENES;
	this->devicePrefabsResourcesPath    = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_PREFABS;
	this->deviceTagsResourcesPath       = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_TAGS;
	this->deviceConstantsResourcesPath  = this->deviceBaseResourcesPath + RESOURCES_FOLDER_NAME_CONSTANTS;
}

void FileSystemUtils::destroy() {
}

#endif // PLATFORM_DESKTOP
