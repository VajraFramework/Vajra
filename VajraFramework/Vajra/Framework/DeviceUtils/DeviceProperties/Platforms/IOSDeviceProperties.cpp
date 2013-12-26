#ifdef PLATFORM_IOS

#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

DeviceProperties::DeviceProperties() {
	this->init();
}

DeviceProperties::~DeviceProperties() {
	this->destroy();
}

void DeviceProperties::init() {
	// TODO [Implement] Actually get the width and height
	this->widthPx  = 1024;
	this->heightPx = 768;
}

void DeviceProperties::destroy() {
}

std::string DeviceProperties::GetOperatingSystem() {
	return "iOS";
}

#endif
