#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/Definitions.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitAnimationManager.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
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
		this->onUnitActionStateChanged((UnitActionState)messageChunk->messageData.iv1.x, (UnitActionState)messageChunk->messageData.iv1.y);
		break;

	case MESSAGE_TYPE_ANIMATION_ENDED_EVENT:
		this->onAnimationEndMessage(messageChunk);
		break;

	default:
		break;
	}
}

void UnitAnimationManager::onAnimationEndMessage(MessageChunk messageChunk) {
	if (messageChunk->messageData.s == UNIT_ANIMATION_CLIP_NAME_postspecial) {
		BaseUnit* thisBaseUnit = this->GetObject()->GetComponent<BaseUnit>();
		VERIFY(thisBaseUnit != nullptr, "UnitAnimationManager's parent game object has a BaseUnit component");
		if(thisBaseUnit->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_POST_SPECIAL) {
			thisBaseUnit->SwitchActionState(UNIT_ACTION_STATE_IDLE);
		}
	}
}

void UnitAnimationManager::onUnitActionStateChanged(UnitActionState oldState, UnitActionState newState) {
	FRAMEWORK->GetLogger()->dbglog("\nUnit action state switched from %d to %d", oldState, newState);
	std::string animclipToPlay;

	switch (newState) {
	case UNIT_ACTION_STATE_IDLE: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_idle;
	} break;

	case UNIT_ACTION_STATE_WALKING: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_walking;
	} break;

	case UNIT_ACTION_STATE_PRE_SPECIAL: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_prespecial;
	} break;
	
	case UNIT_ACTION_STATE_DOING_SPECIAL: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_doingspecial;
	} break;
		
	case UNIT_ACTION_STATE_POST_SPECIAL: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_postspecial;
	} break;	

	default: {
		ASSERT(0, "Valid unit action state %d", newState);
	} break;

	}

	BakedSkeletalAnimation* bakedSkeletalAnimation = this->gameObjectRef->GetComponent<BakedSkeletalAnimation>();
	if (bakedSkeletalAnimation != nullptr && animclipToPlay != "") {
		bakedSkeletalAnimation->PlayAnimationClip(animclipToPlay);
	}
}

void UnitAnimationManager::start() {
	BakedSkeletalAnimation* bakedSkeletalAnimation = this->gameObjectRef->GetComponent<BakedSkeletalAnimation>();
	if (bakedSkeletalAnimation != nullptr) {
		bakedSkeletalAnimation->PlayAnimationClip(UNIT_ANIMATION_CLIP_NAME_idle);
	}
}

void UnitAnimationManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_ENDED_EVENT, this->GetTypeId(), true);

	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");
}

void UnitAnimationManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
