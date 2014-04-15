#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/Switches/GoalZoneSwitch.h"

#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/StringUtilities.h"

#define START_ALPHA 0.5f
#define END_ALPHA 0.9f

// Tween callbacks
void goalZoneSwitchNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams){
	
}

GoalZoneSwitch::GoalZoneSwitch() : UnitInGridZoneSwitch() {
	this->init();
}

GoalZoneSwitch::GoalZoneSwitch(Object* object_) : UnitInGridZoneSwitch(object_) {
	this->init();
}

GoalZoneSwitch::~GoalZoneSwitch() {
	this->destroy();
}

void GoalZoneSwitch::init() {
	this->type = SWITCH_TYPE_WARMUP;
	this->SetResetTime(1.5f);
	
	
	GridZone* gz = this->gameObjectRef->GetComponent<GridZone>();
	ObjectIdType id = gz->VisualzerSpriteRendererId();
	if(id != OBJECT_ID_INVALID) {
		GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
		this->spriteRendererRef = go->GetComponent<SpriteRenderer>();
		this->spriteRendererRef->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, START_ALPHA));
	} else {
		this->spriteRendererRef = nullptr;
	}
}

void GoalZoneSwitch::destroy() {
}

void GoalZoneSwitch::SetRequiredUnitType(std::string typeStr) {
	UnitInGridZoneSwitch::SetRequiredUnitType(typeStr);
}

void GoalZoneSwitch::onUnitEnteredZone(ObjectIdType id) {
	UnitInGridZoneSwitch::onUnitEnteredZone(id);
	if(this->doesObjectCountAsOccupant(id)) {
		MessageData1S1I1F* userParams = new MessageData1S1I1F();
		userParams->i = this->GetObject()->GetId();
		//ENGINE->GetTween()->TweenToNumber(START_ALPHA, END_ALPHA, this->GetResetTime(), INTERPOLATION_TYPE_LINEAR, false, false, true, "goalZoneSwitch" +StringUtilities::ConvertIntToString(userParams->i) , NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, goalZoneSwitchNumberTweenCallback);
	}

}

void GoalZoneSwitch::onUnitExitedZone(ObjectIdType id) {
	UnitInGridZoneSwitch::onUnitExitedZone(id);
	if(this->doesObjectCountAsOccupant(id)) {
		
	}
}

void GoalZoneSwitch::onSwitchCallback(float normalizedTime) {
	if(this->spriteRendererRef != nullptr) {
		float alpha = START_ALPHA + ((END_ALPHA - START_ALPHA) * normalizedTime);
		this->spriteRendererRef->setDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, alpha)); //START_ALPHA + (normalizedTime * END_ALPHA)));
	}		
}