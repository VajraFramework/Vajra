//
//  AiPerception.cpp
//  Created by Matt Kaufmann on 01/17/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"
#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Libraries/glm/gtx/vector_angle.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/GameObject/GameObject.h"

ComponentIdType AiPerception::componentTypeId = COMPONENT_TYPE_ID_AI_PERCEPTION;

AiPerception::AiPerception() : Component() {
	this->init();
}

AiPerception::AiPerception(Object* object_) : Component(object_) {
	this->init();
}

AiPerception::~AiPerception() {
	this->destroy();
}

void AiPerception::Activate() {
	this->knowledge = this->GetObject()->GetComponent<AiKnowledge>();
	ASSERT(this->knowledge != nullptr, "Object with AiPerception also has required component AiKnowledge");
}

void AiPerception::update() {
	this->gatherInformation();
}

void AiPerception::init() {
	this->knowledge     = nullptr;
	this->visionRange   = 1.0f;
	this->fieldOfVision = 45.0f inRadians;
	this->visionAcuity  = 1.0f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void AiPerception::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void AiPerception::gatherInformation() {
	for (int typeInt = FIRST_PLAYER_UNIT_TYPE; typeInt <= LAST_PLAYER_UNIT_TYPE; typeInt++) {
		UnitType uType = static_cast<UnitType>(typeInt);
		ObjectIdType objId = SINGLETONS->GetGridManager()->GetPlayerUnitIdOfType(uType);
		if (objId != OBJECT_ID_INVALID) {
			this->gatherInformationAboutObject(objId);
		}
	}
}

void AiPerception::gatherInformationAboutObject(ObjectIdType objId) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(objId);
	if (gObj != nullptr) {
		float awareness = this->calculateAwarenessOfObject(objId);

		// The AiKnowledge component will collect information based on our level of awareness
		this->knowledge->LearnAboutObject(objId, awareness);
	}
}

float AiPerception::calculateAwarenessOfObject(ObjectIdType objId) {
	// Determine the better awareness by sight or by sound.
	float visual = this->calculateVisualAwareness(objId);
	//float audio = this->calculateAudioAwareness(objId);

	//if (visual >= audio) {
		return visual;
	//}
	//return audio;
}

float AiPerception::calculateVisualAwareness(ObjectIdType objId) {
	Transform* myTrans = this->GetObject()->GetComponent<Transform>();

	GameObject* otherObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(objId);
	Transform* otherTrans = otherObj->GetTransform();

	glm::vec3 myPosition = myTrans->GetPositionWorld();
	glm::vec3 otherPosition = otherTrans->GetPositionWorld();
	glm::vec3 posDiff = otherPosition - myPosition;

	// Is the target in range?
	float targetDistance = glm::length(posDiff);
	if (targetDistance > this->visionRange) {
		return 0.0f;
	}

	if (targetDistance > 0.0f) {
		// Is the AI's vision wide enough?
		glm::vec3 direction = glm::normalize(posDiff);
		glm::vec3 forward = myTrans->GetForward();
		float angle = glm::angle(forward, direction);
		if (angle > this->fieldOfVision) {
			return 0.0f;
		}

		// Is there anything blocking line of sight?
		GridCell* myCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(myPosition);
		GridCell* otherCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(otherPosition);
		if (SINGLETONS->GetGridManager()->GetGrid()->HasLineOfSight(myCell, otherCell)) {
			// I can see it!
			return this->visionAcuity;
		}

		// There's something in the way
		return 0.0f;
	}

	// How did this happen? He's right on top of me!
	return this->visionAcuity;
}

float AiPerception::calculateAudioAwareness(ObjectIdType /*objId*/) {
	return 0.0f;
}
