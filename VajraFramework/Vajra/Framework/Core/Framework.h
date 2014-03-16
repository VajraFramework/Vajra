#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Forward Declarations:
class DeviceProperties;
class FileSystemUtils;
class Logger;
class OpenGLWrapper;
class GLFWwindow;

class Framework {
public:
	static Framework* GetInstance();

	inline Logger*           GetLogger()           { return this->logger;           }
    inline DeviceProperties* GetDeviceProperties() { return this->deviceProperties; }
    inline FileSystemUtils*  GetFileSystemUtils()  { return this->fileSystemUtils;  }
	inline OpenGLWrapper*    GetOpenGLWrapper()    { return this->openGLWrapper;    }

	static GLFWwindow* GetGLFWWindow() { return Framework::window; }
	static void SetGLFWWindow(GLFWwindow* window_) { Framework::window = window_; }

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

	static GLFWwindow* window;
};

#define FRAMEWORK Framework::GetInstance()

#endif // FRAMEWORK_H
