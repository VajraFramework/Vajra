#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"

ComponentIdType MenuManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

MenuManager::MenuManager() : Component() {
	this->init();
}

MenuManager::MenuManager(Object* object_) : Component(object_) {
	this->init();
}

MenuManager::~MenuManager() {
	this->destroy();
}

void MenuManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
}

void MenuManager::init() {

}

void MenuManager::destroy() {
	
}