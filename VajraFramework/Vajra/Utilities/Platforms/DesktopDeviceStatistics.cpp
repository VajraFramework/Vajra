//
//  DesktopDeviceStatistics.cpp
//  Created by Matt Kaufmann on 12/08/13.
//

#ifdef PLATFORM_DESKTOP

#include "Vajra/Utilities/DeviceStatistics.h"

std::string GetOperatingSystem() {
#if PLATFORM_WINDOWS
	return "Windows";
#elif PLATFORM_LINUX
	return "Linux";
#else
	return "UnknownOS";
#endif
}

#endif
