//
//  GridNavigator.cpp
//  Created by Matt Kaufmann on 12/12/13.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

#include "Vajra/Common/Messages/Declarations.h"

ComponentIdType GridNavigator::componentTypeId = COMPONENT_TYPE_ID_GRID_NAVIGATOR;

GridNavigator::GridNavigator() : Component() {
	this->init();
}

GridNavigator::GridNavigator(Object* object_) : Component(object_) {
	this->init();
}

GridNavigator::~GridNavigator() {
	this->destroy();
}

void GridNavigator::init() {

}

void GridNavigator::destroy() {

}

void GridNavigator::HandleMessage(Message* message) {

}
