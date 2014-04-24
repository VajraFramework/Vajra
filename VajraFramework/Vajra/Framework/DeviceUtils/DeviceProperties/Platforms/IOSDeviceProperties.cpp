#ifdef PLATFORM_IOS

#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/Platforms/IOSDevicePropertiesHelper.h"
#include "Vajra/Utilities/StringUtilities.h"

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
	return "iOS";
}


IOS_Device_type_t DeviceProperties::GetIOSDeviceType() {
	std::string machineString = GetMachineName();
	if (machineString.find("iPod") != std::string::npos) {
		return IOS_DEVICE_TYPE_ipod;
	}
	if (machineString.find("iPhone") != std::string::npos) {
		return IOS_DEVICE_TYPE_iphone;
	}
	if (machineString.find("iPad") != std::string::npos) {
		return IOS_DEVICE_TYPE_ipad;
	}
	return IOS_DEVICE_TYPE_unknown;
}

unsigned int DeviceProperties::GetIOSDeviceModelNumber() {
	std::string machineString = GetMachineName();
	std::size_t commaPosition = machineString.find(",");
	if (commaPosition != std::string::npos && commaPosition != 0) {
		std::string modelNumberString = machineString.substr(commaPosition - 1, 1);
		if (!modelNumberString.empty()) {
			return StringUtilities::ConvertStringToInt(modelNumberString);
		}
	}
	// Unknown:
	return 0;
}

#endif
