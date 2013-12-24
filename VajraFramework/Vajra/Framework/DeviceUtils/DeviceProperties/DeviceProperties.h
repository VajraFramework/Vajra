#ifndef DEVICE_PROPERTIES_H
#define DEVICE_PROPERTIES_H

#include "Vajra/Common/Objects/Object.h"

#include <string>

class DeviceProperties : public Object {
public:
	~DeviceProperties();

	std::string GetOperatingSystem();

	inline unsigned int GetWidthPixels()  { return this->widthPx;  }
	inline unsigned int GetHeightPixels() { return this->heightPx; }

private:
	DeviceProperties();
	void init();
	void destroy();

	unsigned int widthPx;
	unsigned int heightPx;

    friend class Framework;
};

#endif // DEVICE_PROPERTIES_H
