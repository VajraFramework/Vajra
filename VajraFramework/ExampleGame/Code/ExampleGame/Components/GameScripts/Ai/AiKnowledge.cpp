//
//  AiKnowledge.cpp
//  Created by Matt Kaufmann on 01/17/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"

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

void AiKnowledge::init() {

}

void AiKnowledge::destroy() {

}

void AiKnowledge::populateInformationMap() {
	// This should determine the list of units we care about and seed the map with basic data.
}
