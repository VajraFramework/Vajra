#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"

Framework* Framework::singleton = nullptr;

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
	this->logger = new Logger();
    this->deviceProperties = new DeviceProperties();
    this->fileSystemUtils = new FileSystemUtils();
	this->openGLWrapper = new OpenGLWrapper();
}
