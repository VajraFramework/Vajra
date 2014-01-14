#ifdef PLATFORM_DESKTOP

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
#if PLATFORM_WINDOWS
	return "Windows";
#elif PLATFORM_LINUX
	return "Linux";
#else
	return "UnknownOS";
#endif
}

#endif
