#ifdef PLATFORM_DESKTOP

#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

DeviceProperties::DeviceProperties() {
	this->init();
}

DeviceProperties::~DeviceProperties() {
	this->destroy();
}

void DeviceProperties::init() {
	// TODO [Hack] Hard coding width and height here
	this->widthPx  = 1024;
	this->heightPx = 768;
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
