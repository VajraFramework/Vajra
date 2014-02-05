#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/UnitAnimations/UnitAnimationManager.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

ComponentIdType UnitAnimationManager::componentTypeId = COMPONENT_TYPE_ID_UNIT_ANIMATION_MANAGER;

UnitAnimationManager::UnitAnimationManager() : Component() {
	this->init();
}

UnitAnimationManager::UnitAnimationManager(Object* object_) : Component(object_) {
	this->init();
}

UnitAnimationManager::~UnitAnimationManager() {
	this->destroy();
}

void UnitAnimationManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED:
		this->onUnitActionStateChanged(UNIT_ACTION_STATE_IDLE, UNIT_ACTION_STATE_WALKING);
		break;

	default:
		break;
	}
}

void UnitAnimationManager::onUnitActionStateChanged(UnitActionState /* oldState */, UnitActionState /* newState */) {
}

void UnitAnimationManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED, this->GetTypeId(), true);
}

void UnitAnimationManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

