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
	Transform* camTransform = ((GameObject*)this->GetObject())->GetTransform();
	camTransform->SetPosition(0.0f, 12.5f, 0.0f);
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
	GameObject* gameObject = (GameObject*)this->GetObject();
	glm::vec3 curPos = gameObject->GetTransform()->GetPosition();
	ENGINE->GetTween()->TweenPosition(gameObject->GetId(), curPos, newPos, 1.0f, ShadyCameraTween::tweenCallback);
}

void ShadyCamera::MoveTo(float x, float y, float z) {
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::PanTo(float x, float z) {
	float y = ((GameObject*)this->GetObject())->GetTransform()->GetPosition().y;
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::ZoomTo(float y) {

}

void ShadyCamera::ZoomBy(float yOffset) {

}

void ShadyCamera::PanToRoom(float x, float z) {

}

void ShadyCamera::PanToRoom(GridCell cell) {

}
 
void ShadyCamera::ZoomToGame() {

}

void ZoomToGame(GridCell cell) {

}

void ShadyCamera::ZoomToOverview() {

}

void ShadyCamera::LevelStartPan() {

}
