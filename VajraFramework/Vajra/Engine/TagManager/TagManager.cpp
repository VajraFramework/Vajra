#include "Vajra/Engine/TagManager/TagManager.h"
#include "Vajra/Framework/Logging/Logger.h"

#include <fstream>

TagManager::TagManager() {
	this->init();
}

TagManager::~TagManager() {
	this->destroy();
}

void TagManager::LoadTagsFromFile(std::string tagFilePath) {
    FRAMEWORK->GetLogger()->dbglog("\nReading in GameObject tags from %s", tagFilePath.c_str());

	std::ifstream tagsFile(tagFilePath.c_str());
	VERIFY_LOG(tagsFile.good(), "Successfully opened tags file %s for reading", tagFilePath.c_str());

	std::string tagName;

	unsigned int i = 0;
	while(!tagsFile.eof()) {
		tagsFile >> tagName;
		stringToBitmask.insert( std::pair<std::string, unsigned int>(tagName, pow(2, i)));
		i++;
	}

    FRAMEWORK->GetLogger()->dbglog("\n Finished reading in GameObject tags");
}

void TagManager::init() {

}

void TagManager::destroy() {

}