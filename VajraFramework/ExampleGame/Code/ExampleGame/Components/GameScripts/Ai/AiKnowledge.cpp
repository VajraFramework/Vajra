//
//  AiKnowledge.cpp
//  Created by Matt Kaufmann on 01/17/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

ComponentIdType AiKnowledge::componentTypeId = COMPONENT_TYPE_ID_AI_KNOWLEDGE;

AiKnowledge::AiKnowledge() : Component() {
	this->init();
}

AiKnowledge::AiKnowledge(Object* object_) : Component(object_) {
	this->init();
}

AiKnowledge::~AiKnowledge() {
	this->destroy();
}

void AiKnowledge::LearnAboutObject(ObjectIdType objId, float awareness) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(objId);
	if (gObj != nullptr) {
		auto iter = this->myKnowledge.find(objId);
		if (iter == this->myKnowledge.end()) {
			this->myKnowledge[objId] = new UnitInformation();
		}

		this->myKnowledge[objId]->Awareness = awareness;

		Transform* trans = gObj->GetTransform();
		ASSERT(trans != nullptr, "GameObject with id %d has Transform component", objId);
		if (awareness >= AI_AWARENESS_THRESHOLD_LOCATION) {
			this->myKnowledge[objId]->Location = trans->GetPositionWorld();
		}

		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if ((awareness >= AI_AWARENESS_THRESHOLD_UNIT_TYPE) && (unit != nullptr)) {
			if (this->myKnowledge[objId]->Type == UNIT_TYPE_UNKNOWN) {
				// The player has been sighted!
				this->myKnowledge[objId]->Type = unit->GetUnitType();

				MessageChunk sightedPlayerMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
				sightedPlayerMessage->SetMessageType(MESSAGE_TYPE_AI_SIGHTED_PLAYER);
				sightedPlayerMessage->messageData.i = objId;
				ENGINE->GetMessageHub()->SendPointcastMessage(sightedPlayerMessage, this->GetObject()->GetId(), objId);
			}
		}
		else {
			if (this->myKnowledge[objId]->Type != UNIT_TYPE_UNKNOWN) {
				// The AI has lost sight of the player
				this->myKnowledge[objId]->Type = UNIT_TYPE_UNKNOWN;

				MessageChunk lostSightOfPlayerMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
				lostSightOfPlayerMessage->SetMessageType(MESSAGE_TYPE_AI_LOST_SIGHT_OF_PLAYER);
				lostSightOfPlayerMessage->messageData.i = objId;
				ENGINE->GetMessageHub()->SendPointcastMessage(lostSightOfPlayerMessage, this->GetObject()->GetId(), objId);
			}
		}
	}
}

void AiKnowledge::init() {

}

void AiKnowledge::destroy() {
	// Release all stored data
	for (auto iter = this->myKnowledge.begin(); iter != this->myKnowledge.end(); ++iter) {
		delete iter->second;
	}
	this->myKnowledge.clear();
}

void AiKnowledge::populateInformationMap() {
	// This should determine the list of units we care about and seed the map with basic data.
}
