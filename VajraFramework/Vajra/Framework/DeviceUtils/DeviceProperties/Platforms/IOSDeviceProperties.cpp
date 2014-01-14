#ifdef PLATFORM_IOS

#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

DeviceProperties::DeviceProperties() {
	this->init();
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
	return "iOS";
}

#endif
