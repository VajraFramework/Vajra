#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitAnimationManager.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

ComponentIdType BaseUnit::componentTypeId = COMPONENT_TYPE_ID_BASE_UNIT;

BaseUnit::BaseUnit() : Component() {
	this->init();
}

BaseUnit::BaseUnit(Object* object_) : Component(object_) {
	this->init();
}

BaseUnit::~BaseUnit() {
	this->destroy();
}

void BaseUnit::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START     , this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT     , this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_END       , this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT, this->GetTypeId(), false);

	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->gridNavRef = this->gameObjectRef->GetComponent<GridNavigator>();
	this->unitState = UnitState::UNIT_STATE_ALIVE;
	this->unitType = UnitType::UNIT_TYPE_GUARD;
	this->unitActionState = UnitActionState::UNIT_ACTION_STATE_IDLE;
}

void BaseUnit::destroy() {
	this->end();
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void BaseUnit::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_UNIT_SPECIAL_HIT:
			this->onUnitSpecialHit(messageChunk->GetSenderId(), messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z, messageChunk->messageData.fv1);
			break;
	}
}

void BaseUnit::start() {
}

void BaseUnit::end() {
	this->gridNavRef = nullptr;
}

void BaseUnit::update() {
}

void BaseUnit::SwitchActionState(UnitActionState newState) {
	if(newState != this->unitActionState) {
		UnitActionState oldState = this->unitActionState;
		this->unitActionState = newState;
		MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
		messageChunk->SetMessageType(MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED);
		messageChunk->messageData.iv1.x = oldState;
		messageChunk->messageData.iv1.y = newState;
		ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->GetObject()->GetId(), this->GetObject()->GetId());
	}
}

void BaseUnit::onUnitSpecialHit(ObjectIdType id, int gridX, int gridZ, glm::vec3 source) {
	// First check if the attack hit my cell
	GridCell* cell = this->gridNavRef->GetCurrentCell();
	if (cell != nullptr) {
		if ((cell->x == gridX) && (cell->z == gridZ)) {
			// Check if the attacker can kill me
			if (this->CanBeKilledBy(id, source)) {
				// I'm dead
				this->Kill();
			}
		}
	}
}

void BaseUnit::Kill() {
	this->unitState = UnitState::UNIT_STATE_DEAD;

	this->gridNavRef->DisableNavigation();

	glm::vec3 pos = this->gameObjectRef->GetTransform()->GetPositionWorld();

	// broadcast a message about unit death
	GridCell* currentCell = this->gridNavRef->GetCurrentCell();
	MessageChunk unitKilledMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	unitKilledMessage->SetMessageType(MESSAGE_TYPE_UNIT_KILLED);
	unitKilledMessage->messageData.iv1.x = currentCell->x;
	unitKilledMessage->messageData.iv1.y = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(pos.y);
	unitKilledMessage->messageData.iv1.z = currentCell->z;
	ENGINE->GetMessageHub()->SendMulticastMessage(unitKilledMessage, this->GetObject()->GetId());

	AudioSource* audioSource = this->gameObjectRef->GetComponent<AudioSource>();
	if (audioSource != nullptr) {
		audioSource->Play("death");
	}
}
