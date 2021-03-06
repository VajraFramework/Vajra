#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/SavedDataManager/SavedDataManager.h"
#include "Vajra/Framework/Settings/Settings.h"

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
	this->logger            = new Logger();

    this->deviceProperties  = new DeviceProperties();
    this->deviceProperties->init();

    this->fileSystemUtils   = new FileSystemUtils();
    this->fileSystemUtils->init();

	this->settings          = new Settings();
	this->settings->init();

	this->savedDataManager  = new SavedDataManager();
	this->savedDataManager->init();

	this->openGLWrapper     = new OpenGLWrapper();
	this->openGLWrapper->init();
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
	if (this->savedDataManager != nullptr) {
		delete this->savedDataManager;
	}
	if (this->settings != nullptr) {
		delete this->settings;
	}
	if (this->logger != nullptr) {
		delete this->logger;
	}
}
