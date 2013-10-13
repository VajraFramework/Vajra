#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include "Vajra/Common/Objects/Object.h"

#include <string>

class FileSystemUtils : public Object {
public:
    ~FileSystemUtils();
    
    inline const std::string GetDeviceBaseResourcesPath() const { return this->deviceBaseResourcesPath; }
    inline const std::string GetDevicePictureResourcesPath() const { return this->devicePictureResourcesPath; }
    inline const std::string GetDeviceShaderResourcesPath() const { return this->deviceShaderResourcesPath; }
    
private:
    FileSystemUtils();
    void init();
    void destroy();
    
    std::string deviceBaseResourcesPath;
    std::string devicePictureResourcesPath;
    std::string deviceShaderResourcesPath;
    
    friend class Framework;
};

#endif // FILESYSTEMUTILS_H
