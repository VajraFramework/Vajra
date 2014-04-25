#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"
#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/TutorialCache.h"
#include "ExampleGame/Messages/Declarations.h"

ComponentIdType TutorialCache::componentTypeId = COMPONENT_TYPE_ID_TUTORIAL_CACHE;

TutorialCache::TutorialCache() : Component() {
	this->init();
}

TutorialCache::TutorialCache(Object* object_) : Component(object_)  {
	this->init();
}

TutorialCache::~TutorialCache() {
	this->destroy();
}

void TutorialCache::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_PRELOAD_IMAGE: {
		this->preloadImage(messageChunk->messageData.s);
	} break;

	default : {
	} break;

	}
}

void TutorialCache::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PRELOAD_IMAGE, this->GetTypeId(), false);
}

void TutorialCache::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void TutorialCache::preloadImage(std::string imageName) {
	this->cachedTutorialImageAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
}

void TutorialCache::purgeCache() {
	this->cachedTutorialImageAsset = 0;
}
