#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"


namespace ShadyCameraTween {
	void tweenCallback(ObjectIdType gameObjectId, std::string tweenClipName) {
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

void ShadyCamera::HandleMessage(Message* message) {
	Camera::HandleMessage(message);
	
}
void ShadyCamera::init() {
	// Set camera properties
	this->SetFOV(30.0f);
	this->gameObject = (GameObject*)this->GetObject();
	Transform* camTransform = this->gameObject->GetTransform();
	// TODO [Implement] : set this based on device resolution
	this->gameCamHeight = 24.0f;
	// TODO [Implement] : get overview pos and start room from level data
	this->overviewPos = glm::vec3(18.0f, 50.0f, -18.0f);
	this->gameCamPos = glm::vec3(0.0f, this->gameCamHeight, 0.0f);

	camTransform->SetPosition(this->gameCamPos);
	camTransform->Rotate(90.0f, XAXIS);
	camTransform->Rotate(180.0f, camTransform->GetForward());

	this->gridManager = nullptr;
}

void ShadyCamera::destroy() {
	this->gridManager = nullptr;
}

void ShadyCamera::setGridManager(GridManager* newManager) {
	this->gridManager = newManager;
}

void ShadyCamera::MoveTo(glm::vec3 newPos) {
	glm::vec3 curPos = this->gameObject->GetTransform()->GetPosition();
	// TODO [HACK] : remove when Tween is changed
	this->gameObject->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	ENGINE->GetTween()->TweenPosition(this->gameObject->GetId(), curPos, newPos, 3.0f, ShadyCameraTween::tweenCallback);
}

void ShadyCamera::MoveTo(float x, float y, float z) {
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::PanTo(float x, float z) {
	float y = this->gameObject->GetTransform()->GetPosition().y;
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::ZoomTo(float y) {
	glm::vec3 newPos = this->gameObject->GetTransform()->GetPosition();
	newPos.y = y;
	this->MoveTo(newPos);
}

void ShadyCamera::ZoomBy(float yOffset) {
	glm::vec3 newPos = this->gameObject->GetTransform()->GetPosition();
	newPos.y += yOffset;
	this->MoveTo(newPos);
}
 
void ShadyCamera::MoveToRoom() {
	this->MoveTo(this->gameCamPos);
}

void ShadyCamera::MoveToRoom(float x, float z) {
	this->SetGameCam(x, z);
	this->MoveToRoom();
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

void ShadyCamera::SetGameCam(float x, float z) {
	this->gameCamPos = this->gridManager->GetRoomCenter(x, z);
	this->gameCamPos.y += this->gameCamHeight;
}
