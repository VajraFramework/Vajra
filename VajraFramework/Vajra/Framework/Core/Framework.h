#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Forward Declarations:
class DeviceProperties;
class FileSystemUtils;
class Logger;
class OpenGLWrapper;
class Settings;
class GLFWwindow;

class Framework {
public:
	static Framework* GetInstance();

	inline Logger*           GetLogger()           { return this->logger;           }
    inline DeviceProperties* GetDeviceProperties() { return this->deviceProperties; }
    inline FileSystemUtils*  GetFileSystemUtils()  { return this->fileSystemUtils;  }
	inline OpenGLWrapper*    GetOpenGLWrapper()    { return this->openGLWrapper;    }
	inline Settings*         GetSettings()         { return this->settings;         }

	static GLFWwindow* GetGLFWWindow() { return Framework::window; }
	static void SetGLFWWindow(GLFWwindow* window_) { Framework::window = window_; }

private:
	// Make this a singleton:
	Framework();
	static Framework* singleton;
	//
	void init();
	void destroy();

	Logger* logger;
	DeviceProperties* deviceProperties;
    FileSystemUtils* fileSystemUtils;
	OpenGLWrapper* openGLWrapper;
	Settings* settings;
	
	static GLFWwindow* window;
};

#define FRAMEWORK Framework::GetInstance()

#endif // FRAMEWORK_H
