#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Forward Declarations:
class DeviceProperties;
class FileSystemUtils;
class Logger;
class OpenGLWrapper;

class Framework {
public:
	static Framework* GetInstance();

	inline Logger*           GetLogger()           { return this->logger;           }
    inline DeviceProperties* GetDeviceProperties() { return this->deviceProperties; }
    inline FileSystemUtils*  GetFileSystemUtils()  { return this->fileSystemUtils;  }
	inline OpenGLWrapper*    GetOpenGLWrapper()    { return this->openGLWrapper;    }

private:
	// Make this a singleton:
	Framework();
	static Framework* singleton;
	//
	void init();

	Logger* logger;
	DeviceProperties* deviceProperties;
    FileSystemUtils* fileSystemUtils;
	OpenGLWrapper* openGLWrapper;
};

#define FRAMEWORK Framework::GetInstance()

#endif // FRAMEWORK_H
