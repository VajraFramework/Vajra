#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/Definitions.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitAnimationManager.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/StringUtilities.h"

#define DEATH_FADEOUT_TIME 1.0f

ComponentIdType UnitAnimationManager::componentTypeId = COMPONENT_TYPE_ID_UNIT_ANIMATION_MANAGER;

void deathFadeTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* userParams) {
	GameObject* caller = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);

	// Make sure the object is still around
	if (caller != nullptr) {
		caller->SetVisible(false);
	}
}

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
	BaseUnit* thisBaseUnit = this->GetObject()->GetComponent<BaseUnit>();
	//VERIFY(thisBaseUnit != nullptr, "UnitAnimationManager's parent game object has a BaseUnit component");
	if (thisBaseUnit != nullptr) {
		if (messageChunk->messageData.s == UNIT_ANIMATION_CLIP_NAME_postspecial) {
			if (thisBaseUnit != nullptr) {
				if (thisBaseUnit->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_POST_SPECIAL) {
					thisBaseUnit->SwitchActionState(UNIT_ACTION_STATE_IDLE);
				}
			}
		}
		else if (messageChunk->messageData.s == UNIT_ANIMATION_CLIP_NAME_death) {
			if (thisBaseUnit->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_DEATH) {
				std::string tweenName = "DeathTimeout" + StringUtilities::ConvertIntToString(this->GetObject()->GetId());
				MessageData1S1I1F* userParams = new MessageData1S1I1F();
				userParams->i = this->GetObject()->GetId();
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, DEATH_FADEOUT_TIME, INTERPOLATION_TYPE_LINEAR, false, false, false, tweenName, NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, deathFadeTweenCallback);
			}
		}
	}
}

void UnitAnimationManager::onUnitActionStateChanged(UnitActionState /*oldState*/, UnitActionState newState) {
	// FRAMEWORK->GetLogger()->dbglog("\nUnit action state switched from %d to %d", oldState, newState);
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

	case UNIT_ACTION_STATE_PRE_BLOCK: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_block;
	} break;

	case UNIT_ACTION_STATE_BLOCK_IDLE: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_block_idle;
	} break;

	case UNIT_ACTION_STATE_DEATH: {
		animclipToPlay = UNIT_ANIMATION_CLIP_NAME_death;
	} break;

	default: {
		ASSERT(0, "Valid unit action state %d", newState);
	} break;

	}

	BakedSkeletalAnimation* bakedSkeletalAnimation = this->gameObjectRef->GetComponent<BakedSkeletalAnimation>();
	if (bakedSkeletalAnimation != nullptr && animclipToPlay != "") {
		bakedSkeletalAnimation->PlayAnimationClip(animclipToPlay);
	}
	RigidAnimation* rigidAnimation = this->gameObjectRef->GetComponent<RigidAnimation>();
	if (rigidAnimation != nullptr && animclipToPlay != "") {
		rigidAnimation->PlayAnimationClip(animclipToPlay);
	}
}

void UnitAnimationManager::start() {
	BakedSkeletalAnimation* bakedSkeletalAnimation = this->gameObjectRef->GetComponent<BakedSkeletalAnimation>();
	if (bakedSkeletalAnimation != nullptr) {
		bakedSkeletalAnimation->PlayAnimationClip(UNIT_ANIMATION_CLIP_NAME_idle);
	}
	RigidAnimation* rigidAnimation = this->gameObjectRef->GetComponent<RigidAnimation>();
	if (rigidAnimation != nullptr) {
		rigidAnimation->PlayAnimationClip(UNIT_ANIMATION_CLIP_NAME_idle);
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
