#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/Settings/Settings.h"

Framework* Framework::singleton = nullptr;
GLFWwindow* Framework::window = nullptr;

Framework::Framework() {
	// Singletons don't call init() in constructor, they call it in GetInstance()
}

// static method
Framework* Framework::GetInstance() {
	if (Framework::singleton == 0) {
		Framework::singleton = new Framework();
		Framework::singleton->init();
	}

	return Framework::singleton;
}

void Framework::init() {
	this->logger            = new Logger();
    this->deviceProperties  = new DeviceProperties();
    this->fileSystemUtils   = new FileSystemUtils();
	this->settings          = new Settings();
	this->openGLWrapper     = new OpenGLWrapper();
}

void Framework::destroy() {
    if (this->deviceProperties != nullptr) {
    	delete this->deviceProperties;
    }
    if (this->fileSystemUtils != nullptr) {
    	delete this->fileSystemUtils;
    }
	if (this->openGLWrapper != nullptr) {
		delete this->openGLWrapper;
	}
	if (this->settings != nullptr) {
		delete this->settings;
	}
	if (this->logger != nullptr) {
		delete this->logger;
	}
}
