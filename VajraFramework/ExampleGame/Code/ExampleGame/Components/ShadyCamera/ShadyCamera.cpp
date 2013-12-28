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
	this->gameCamHeight = 12.5f;
	// TODO [Implement] : get this from level data
	this->overviewPos = glm::vec3(18.0f, 30.0f, -18.0f);
	camTransform->SetPosition(0.0f, gameCamHeight, 0.0f);
	camTransform->Rotate(90.0f, XAXIS);
	camTransform->Rotate(180.0f, camTransform->GetForward());

	this->gridManager = nullptr;
}

void ShadyCamera::destroy() {
	this->gridManager = nullptr;
}

void ShadyCamera::setGridManager(GridManager* newManager) {
	this->gridManager = newManager;
	if(newManager == nullptr)
		return;
  
}

void ShadyCamera::MoveTo(glm::vec3 newPos) {
	glm::vec3 curPos = this->gameObject->GetTransform()->GetPosition();
	ENGINE->GetTween()->TweenPosition(gameObject->GetId(), curPos, newPos, 3.0f, ShadyCameraTween::tweenCallback);
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

void ShadyCamera::PanToRoom(float x, float z) {
	float roomX = this->gridManager->GetRoomX(x);
	float roomZ = this->gridManager->GetRoomZ(z);
	float roomWidth = this->gridManager->GetRoomWidth();
	float roomHeight = this->gridManager->GetRoomHeight();
	roomX *= roomWidth;
	roomZ *= roomHeight;
	roomX += roomWidth / 2.0f;
	roomZ += roomHeight / -2.0f;
	float cellHalfSize = this->gridManager->getCellSize() / 2.0f;
	roomX -= cellHalfSize;
	roomZ += cellHalfSize;
	this->PanTo(roomX, roomZ);

	// store the last room
	this->lastRoomPos = glm::vec3(roomX, this->gameCamHeight, roomZ);
}

void ShadyCamera::PanToRoom(GridCell cell) {
	this->PanTo(cell.x, cell.z);
}
 
void ShadyCamera::ZoomToGame() {
	this->MoveTo(this->lastRoomPos);
}

void ShadyCamera::ZoomToGame(GridCell cell) {
	this->PanToRoom(cell);
	this->ZoomTo(this->gameCamHeight);
}

void ShadyCamera::ZoomToOverview() {
	this->MoveTo(this->overviewPos);
}

// TODO [Implement] : if needed
void ShadyCamera::LevelStartPan() {

}
