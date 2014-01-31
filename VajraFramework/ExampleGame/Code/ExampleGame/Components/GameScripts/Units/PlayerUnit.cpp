#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"

#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"

PlayerUnit::PlayerUnit() : BaseUnit() {
	this->init();
}

PlayerUnit::PlayerUnit(Object* object_) : BaseUnit(object_) {
	this->init();
}

PlayerUnit::~PlayerUnit() {
	this->destroy();
}

void PlayerUnit::init() {
	this->unitType = UnitType::UNIT_TYPE_ASSASSIN;
	this->inputState = InputState::INPUT_STATE_WAIT;

	// DECAL TEST
	this->touchIndicator = PrefabLoader::InstantiateGameObjectFromPrefab(
								FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "decal.prefab",
								ENGINE->GetSceneGraph3D());
}

void PlayerUnit::destroy() {
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	this->touchIndicator->GetTransform()->SetPosition(touchedCell->center);
	switch(this->inputState) {
		case InputState::INPUT_STATE_WAIT:
			this->onSelectedTouch();
			break;
		case InputState::INPUT_STATE_NAV:
			this->onNavTouch(touchId, touchedCell);
			break;
		case InputState::INPUT_STATE_SPECIAL:
			this->onSpecialTouch(touchId);
			break;
		default:
			break;
	}
}

void PlayerUnit::OnDeselect() {
	this->inputState = InputState::INPUT_STATE_WAIT;
}
void PlayerUnit::onSelectedTouch() {
	this->inputState = InputState::INPUT_STATE_NAV;
}

void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	if(this->isSpecialTouch(touchId)) {

	}
	else {
		if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Ended) {
			this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
		}
	}

}
