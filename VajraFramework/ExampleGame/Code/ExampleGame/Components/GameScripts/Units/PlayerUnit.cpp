#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"

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
	this->touchIndicator = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->touchIndicator->AddComponent<SpriteRenderer>();

	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_Touch_Bad.png");
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures);
	//
	this->touchIndicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
	this->touchIndicator->GetTransform()->SetPosition(0.0f, 0.0f, -1.0f);
}

void PlayerUnit::destroy() {
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	this->touchIndicator->GetTransform()->SetPosition(touchedCell->center + glm::vec3(0.0f, 0.0f, 0.0f));
	DebugDraw::DrawCube(touchedCell->center, 1.0f);
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
