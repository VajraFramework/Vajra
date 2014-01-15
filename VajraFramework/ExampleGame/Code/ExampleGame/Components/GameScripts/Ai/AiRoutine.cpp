//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"

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

void AiRoutine::init() {

}

void AiRoutine::destroy() {

}

void AiRoutine::parseTaskStrings() {
	/*
	// Clear the current list of tasks.
	markers.clear();

	// Parse list of commands.
	for (unsigned int i = 0; i < this->taskStrings.size(); ++i) {
		int x1, z1, x2, z2;
		float t;
		std::string tag;
		switch (tag) {
			case "START":
				this->processStart(x1, z1, x2, z2);
				break;

			case "WALK":
				this->processWalk(x1, z1);
				this->processWait(t);
				break;

			case "LOOK":
				this->processLook(x1, z1);
				this->processWait(t);
				break;

			case "WAIT":
				this->processWait(t);
				break;
		}
	}

	this->currentMarker = this->markers.size() - 1;
	this->resumeSchedule();
	*/
}

void AiRoutine::processStart(int /*xPos*/, int /*zPos*/, int /*xLook*/, int /*zLook*/) {

}

void AiRoutine::processWalk(int /*xDiff*/, int /*zDiff*/) {

}

void AiRoutine::processLook(int /*xDiff*/, int /*zDiff*/) {

}

void AiRoutine::processWait(float /*duration*/) {

}

void AiRoutine::resumeSchedule() {

}
