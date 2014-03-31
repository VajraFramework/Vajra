#include "ExampleGame/Components/Triggers/TriggerLoot.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

TriggerLoot::TriggerLoot() : Triggerable() {
	this->init();
}

TriggerLoot::TriggerLoot(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerLoot::~TriggerLoot() {
	this->destroy();
}

void TriggerLoot::init() {
	Triggerable::SetTriggerType("Once");
}

void TriggerLoot::destroy() {

}

void TriggerLoot::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);
}

void TriggerLoot::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerLoot::onSwitchToggled(bool switchState) {
	//this->startTransformation(switchState);
}
