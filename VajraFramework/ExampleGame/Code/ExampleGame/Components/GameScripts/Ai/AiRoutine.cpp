//
//  AiRoutine.cpp
//  Created by Matt Kaufmann on 01/15/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "Libraries/glm/gtx/quaternion.hpp"
#include "Libraries/glm/gtx/vector_angle.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/StringUtilities.h"

ComponentIdType AiRoutine::componentTypeId = COMPONENT_TYPE_ID_AI_ROUTINE;

AiRoutine::AiRoutine() : Component() {
	this->init();
}

AiRoutine::AiRoutine(Object* object_) : Component(object_) {
	this->init();
}

AiRoutine::~AiRoutine() {
	this->destroy();
}

void AiRoutine::SetBehavior(std::vector<std::string>& commands) {
	// Clear the current list of tasks
	this->markers.clear();
	ParseAiCommands(commands, this->markers);

	this->waitTimer = 0.0f;
	this->currentMarker = 0;
	this->ResumeSchedule();
}

void AiRoutine::Follow() {
	GridNavigator* gNav = this->GetObject()->GetComponent<GridNavigator>();

	if (this->currentMarkerType == AI_MARKER_WALK) {
		if (!gNav->IsTraveling()) {
			this->nextMarker();
		}
	}
	else if (this->currentMarkerType == AI_MARKER_LOOK) {
		if (!gNav->IsTurning()) {
			this->nextMarker();
		}
	}
	else if (this->currentMarkerType == AI_MARKER_WAIT) {
		this->waitTimer -= ENGINE->GetTimer()->GetDeltaFrameTime();
		if (this->waitTimer <= 0.0f) {
			this->nextMarker();
		}
	}
}

void AiRoutine::init() {
	this->unit = this->GetObject()->GetComponent<BaseUnit>();
	ASSERT(this->unit != nullptr, "AiRoutine attached to object %d with BaseUnit component", this->GetObject()->GetId());
}

void AiRoutine::destroy() {

}

void AiRoutine::nextMarker() {
	this->currentMarker = (this->currentMarker + 1) % this->markers.size();

	this->ResumeSchedule();
}

void AiRoutine::ResumeSchedule() {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	AiMarker prevMark;
	prevMark.WaitTime = this->waitTimer;
	prevMark.Position = trans->GetPositionWorld();
	prevMark.Orientation = trans->GetOrientationWorld();
	AiMarker newMark = this->markers[this->currentMarker];

	glm::vec3 oldForward = QuaternionForwardVector(prevMark.Orientation);
	glm::vec3 newForward = QuaternionForwardVector(newMark.Orientation);

	float dist = glm::distance(prevMark.Position, newMark.Position);
	float angle = glm::angle(oldForward, newForward);
	GridNavigator* gNav = this->GetObject()->GetComponent<GridNavigator>();
	if (dist > ROUNDING_ERROR) {
		this->currentMarkerType = AI_MARKER_WALK;
		gNav->SetDestination(newMark.Position);
		this->unit->SwitchActionState(UNIT_ACTION_STATE_WALKING);
	}
	else if (angle > ROUNDING_ERROR) {
		this->currentMarkerType = AI_MARKER_LOOK;
		gNav->SetLookTarget(newMark.Orientation);
		this->unit->SwitchActionState(UNIT_ACTION_STATE_IDLE);
	}
	else {
		this->currentMarkerType = AI_MARKER_WAIT;
		this->waitTimer = newMark.WaitTime;
		this->unit->SwitchActionState(UNIT_ACTION_STATE_IDLE);
	}
}
