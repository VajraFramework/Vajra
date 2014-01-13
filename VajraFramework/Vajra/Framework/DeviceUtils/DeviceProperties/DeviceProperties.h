#ifndef DEVICE_PROPERTIES_H
#define DEVICE_PROPERTIES_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/Definitions.h"

#include <string>

class DeviceProperties : public Object {
public:
	~DeviceProperties();

	std::string GetOperatingSystem();

	void SetWidthPixels(unsigned int w)   { this->widthPx = w;   this->computeDeviceResolutionSlab(); }
	void SetHeightPixels(unsigned int h)  { this->heightPx = h;  this->computeDeviceResolutionSlab(); }
	
	inline unsigned int GetWidthPixels()  { return this->widthPx;  }
	inline unsigned int GetHeightPixels() { return this->heightPx; }

	DeviceResolutionSlab GetDeviceResolutionSlab () { return this->deviceResolutionSlab; }

private:
	DeviceProperties();
	void init();
	void destroy();

	// Utility Functions:
	inline void computeDeviceResolutionSlab();

	unsigned int widthPx;
	unsigned int heightPx;

	DeviceResolutionSlab deviceResolutionSlab;

    friend class Framework;
};

////////////////////////////////////////////////////////////////////////////////

void DeviceProperties::computeDeviceResolutionSlab() {
	if (this->widthPx != 0 && this->heightPx != 0) {
		float aspectRatio = (float)this->widthPx / (float)this->heightPx;
		aspectRatio += 0.001f;	// Add a little bit to help the floating point comparisons

		// Check aspect ratios in ascending order:
		//
		if (aspectRatio >= (4.0f / 3.0f)) {
			this->deviceResolutionSlab = DEVICE_RESOLUTION_SLAB__4_3;
		} else {
			ASSERT(0, "Supported aspect ratio: width %u, height %u", this->widthPx, this->heightPx);
		}
		//
		if (aspectRatio >= (3.0f / 2.0f)) {
			this->deviceResolutionSlab = DEVICE_RESOLUTION_SLAB__3_2;
		} else {
			return;
		}
		//
		if (aspectRatio >= (17.0f / 10.0f)) {
			this->deviceResolutionSlab = DEVICE_RESOLUTION_SLAB_17_10;
		} else {
			return;
		}
		//
		if (aspectRatio >= (16.0f / 9.0f)) {
			this->deviceResolutionSlab = DEVICE_RESOLUTION_SLAB_16_9;
		} else {
			return;
		}
	}
}

#endif // DEVICE_PROPERTIES_H
