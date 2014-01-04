#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

namespace ShadyCameraTween {
	void tweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */) {
	}
}

ShadyCamera::ShadyCamera() : Camera() {
	this->init();
}

ShadyCamera::ShadyCamera(Object* object_) : Camera(object_) {
	this->init();
}

ShadyCamera::~ShadyCamera() {
	this->destroy();
}

void ShadyCamera::init() {
	// Set camera properties
	this->SetFOV(30.0f);
	this->camMode = CameraMode::CAMERA_MODE_GAME;
	this->camSpeed = 3.0f;
	this->gameObjectRef = (GameObject*)this->GetObject();
	Transform* camTransform = this->gameObjectRef->GetTransform();

	// TODO [Implement] : set this based on device resolution
	this->gameCamHeight = 24.0f;

	// TODO [Implement] : get overview pos and start room from level data
	this->overviewPos = glm::vec3(18.0f, 50.0f, -18.0f);
	this->gameCamPos = glm::vec3(0.0f, this->gameCamHeight, 0.0f);

	camTransform->SetPosition(this->gameCamPos);
	camTransform->Rotate(90.0f, XAXIS);
	camTransform->Rotate(180.0f, camTransform->GetForward());

	this->gridManagerRef = nullptr;
}

void ShadyCamera::destroy() {
	this->gameObjectRef = nullptr;
	this->gridManagerRef = nullptr;
}

void ShadyCamera::HandleMessage(MessageChunk messageChunk) {
	Camera::HandleMessage(messageChunk);
}

void ShadyCamera::SetGridManager(GridManager* newManager) {
	this->gridManagerRef = newManager;
}

void ShadyCamera::MoveTo(glm::vec3 newPos) {
	glm::vec3 curPos = this->gameObjectRef->GetTransform()->GetPosition();
	// TODO [HACK] : remove when Tween is changed
	this->gameObjectRef->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(), curPos, newPos, this->camSpeed, ShadyCameraTween::tweenCallback);
}

void ShadyCamera::MoveTo(float x, float y, float z) {
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::PanTo(float x, float z) {
	float y = this->gameObjectRef->GetTransform()->GetPosition().y;
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::ZoomTo(float y) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y = y;
	this->MoveTo(newPos);
}

void ShadyCamera::ZoomBy(float yOffset) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y += yOffset;
	this->MoveTo(newPos);
}
 
void ShadyCamera::MoveToCurrentRoom() {
	this->MoveTo(this->gameCamPos);
}

void ShadyCamera::MoveToRoom(float x, float z) {
	this->setGameCameraPosition(x, z);
	this->MoveToCurrentRoom();
}

void ShadyCamera::MoveToRoom(GridCell* cell) {
	this->MoveToRoom(cell->x, cell->z);
}

void ShadyCamera::MoveToOverview() {
	this->MoveTo(this->overviewPos);
}

// TODO [Implement] : if needed
void ShadyCamera::LevelStartPan() {

}

void ShadyCamera::setGameCameraPosition(float x, float z) {
	this->gameCamPos = this->gridManagerRef->GetRoomCenter(x, z);
	this->gameCamPos.y += this->gameCamHeight;
}
