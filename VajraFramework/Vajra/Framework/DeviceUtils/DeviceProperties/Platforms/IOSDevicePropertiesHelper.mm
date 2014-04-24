#include "Vajra/Framework/DeviceUtils/DeviceProperties/Platforms/IOSDevicePropertiesHelper.h"
#import <sys/utsname.h>

static NSString* machineName() {
	struct utsname systemInfo;
	uname(&systemInfo);

	return [NSString stringWithCString:systemInfo.machine
		encoding:NSUTF8StringEncoding];
}

std::string GetMachineName() {
	NSString *s = machineName();
	const char *c = [s UTF8String];
	std::string returnString = c;

	return returnString;
}
