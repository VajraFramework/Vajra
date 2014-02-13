#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include "Vajra/Common/Objects/Object.h"

#include <string>

#if !defined(PLATFORM_IOS)
#define RESOURCES_FOLDER_NAME_PICTURES            "pictures/"
#else
#define RESOURCES_FOLDER_NAME_PICTURES            "pictures_ios_READONLY/"
#endif
#define RESOURCES_FOLDER_NAME_SHADERS             "shaders/"
#define RESOURCES_FOLDER_NAME_MODELS              "models/"
#define RESOURCES_FOLDER_NAME_ARMATURES           "armatures/"
#define RESOURCES_FOLDER_NAME_ANIMATIONS          "animations/"
#define RESOURCES_FOLDER_NAME_AUDIO               "audio/"
#define RESOURCES_FOLDER_NAME_LOGGING             "logging/"
#if !defined(PLATFORM_IOS)
#define RESOURCES_FOLDER_NAME_FONTS               "fonts/"
#else
#define RESOURCES_FOLDER_NAME_FONTS               "fonts_ios_READONLY/"
#endif
#define RESOURCES_FOLDER_NAME_UISCENES            "uiscenes/"
#define RESOURCES_FOLDER_NAME_PREFABS             "prefabs/"
#define RESOURCES_FOLDER_NAME_TAGS                "tags/"
#define RESOURCES_FOLDER_NAME_CONSTANTS           "gameconstants/"

class FileSystemUtils : public Object {
public:
    ~FileSystemUtils();

    inline const std::string GetDevicePictureResourcesFolderName()   const { return RESOURCES_FOLDER_NAME_PICTURES;   }
    inline const std::string GetDeviceShaderResourcesFolderName()    const { return RESOURCES_FOLDER_NAME_SHADERS;    }
    inline const std::string GetDeviceModelResourcesFolderName()     const { return RESOURCES_FOLDER_NAME_MODELS;     }
    inline const std::string GetDeviceArmatureResourcesFolderName()  const { return RESOURCES_FOLDER_NAME_ARMATURES;  }
    inline const std::string GetDeviceAnimationResourcesFolderName() const { return RESOURCES_FOLDER_NAME_ANIMATIONS; }
    inline const std::string GetDeviceAudioResourcesFolderName()     const { return RESOURCES_FOLDER_NAME_AUDIO;      }
    inline const std::string GetDeviceLoggingResourcesFolderName()   const { return RESOURCES_FOLDER_NAME_LOGGING;    }
    inline const std::string GetDeviceFontResourcesFolderName()      const { return RESOURCES_FOLDER_NAME_FONTS;      }
    inline const std::string GetDeviceUiScenesResourcesFolderName()  const { return RESOURCES_FOLDER_NAME_UISCENES;   }
    inline const std::string GetDevicePrefabsResourcesFolderName()   const { return RESOURCES_FOLDER_NAME_PREFABS;    }
    inline const std::string GetDevicePrefabsTagsFolderName()        const { return RESOURCES_FOLDER_NAME_TAGS;       }
    inline const std::string GetDevicePrefabsConstantsFolderName()   const { return RESOURCES_FOLDER_NAME_CONSTANTS;  }
    
    inline const std::string GetDeviceBaseResourcesPath()      const { return this->deviceBaseResourcesPath;          }
    inline const std::string GetDevicePictureResourcesPath()   const { return this->devicePictureResourcesPath;       }
    inline const std::string GetDeviceShaderResourcesPath()    const { return this->deviceShaderResourcesPath;        }
    inline const std::string GetDeviceModelResourcesPath()     const { return this->deviceModelResourcesPath;         }
    inline const std::string GetDeviceArmatureResourcesPath()  const { return this->deviceArmatureResourcesPath;      }
    inline const std::string GetDeviceAnimationResourcesPath() const { return this->deviceAnimationResourcesPath;     }
    inline const std::string GetDeviceAudioResourcesPath()     const { return this->deviceAudioResourcesPath;         }
    inline const std::string GetDeviceLoggingResourcesPath()   const { return this->deviceLoggingResourcesPath;       }
    inline const std::string GetDeviceFontResourcesPath()      const { return this->deviceFontResourcesPath;          }
    inline const std::string GetDeviceUiScenesResourcesPath()  const { return this->deviceUiScenesResourcesPath;      }
    inline const std::string GetDevicePrefabsResourcesPath()   const { return this->devicePrefabsResourcesPath;       }
    inline const std::string GetDeviceTagsResourcesPath()      const { return this->deviceTagsResourcesPath;          }
    inline const std::string GetDeviceConstantsResourcesPath() const { return this->deviceConstantsResourcesPath;     }
    
private:
    FileSystemUtils();
    void init();
    void destroy();
    
    std::string deviceBaseResourcesPath;
    std::string devicePictureResourcesPath;
    std::string deviceShaderResourcesPath;
    std::string deviceModelResourcesPath;
    std::string deviceArmatureResourcesPath;
    std::string deviceAnimationResourcesPath;
    std::string deviceAudioResourcesPath;
    std::string deviceLoggingResourcesPath;
    std::string deviceFontResourcesPath;
    std::string deviceUiScenesResourcesPath;
    std::string devicePrefabsResourcesPath;
    std::string deviceTagsResourcesPath;
    std::string deviceConstantsResourcesPath;
    
    friend class Framework;
};

#endif // FILESYSTEMUTILS_H
