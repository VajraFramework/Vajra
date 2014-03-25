#ifdef PLATFORM_ANDROID

#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

DeviceProperties::DeviceProperties() {
	// Do not call init() here
}

DeviceProperties::~DeviceProperties() {
	this->destroy();
}

void DeviceProperties::init() {
	this->widthPx  = 0;
	this->heightPx = 0;
}

void DeviceProperties::destroy() {
}

std::string DeviceProperties::GetOperatingSystem() {
	return "Android";
}

#endif
