#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include "Vajra/Common/Objects/Object.h"

#include <string>

class FileSystemUtils : public Object {
public:
    ~FileSystemUtils();
    
    inline const std::string GetDeviceBaseResourcesPath()      const { return this->deviceBaseResourcesPath;      }
    inline const std::string GetDevicePictureResourcesPath()   const { return this->devicePictureResourcesPath;   }
    inline const std::string GetDeviceShaderResourcesPath()    const { return this->deviceShaderResourcesPath;    }
    inline const std::string GetDeviceModelResourcesPath()     const { return this->deviceModelResourcesPath;     }
    inline const std::string GetDeviceAnimationResourcesPath() const { return this->deviceAnimationResourcesPath; }
	inline const std::string GetDeviceAudioResourcesPath()     const { return this->deviceAudioResourcesPath; }
    
private:
    FileSystemUtils();
    void init();
    void destroy();
    
    std::string deviceBaseResourcesPath;
    std::string devicePictureResourcesPath;
    std::string deviceShaderResourcesPath;
    std::string deviceModelResourcesPath;
    std::string deviceAnimationResourcesPath;
	std::string deviceAudioResourcesPath;
    
    friend class Framework;
};

#endif // FILESYSTEMUTILS_H
