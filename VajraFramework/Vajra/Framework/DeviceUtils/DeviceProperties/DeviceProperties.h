#ifndef DEVICE_PROPERTIES_H
#define DEVICE_PROPERTIES_H

#include "Vajra/Common/Objects/Object.h"

#include <string>

class DeviceProperties : public Object {
public:
	~DeviceProperties();

	std::string GetOperatingSystem();

	void SetWidthPixels(unsigned int w)   { this->widthPx = w;  }
	void SetHeightPixels(unsigned int h)  { this->heightPx = h; }
	
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
