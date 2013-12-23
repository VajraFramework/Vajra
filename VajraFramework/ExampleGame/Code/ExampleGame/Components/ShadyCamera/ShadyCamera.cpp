#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"

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

	this->gridManager = nullptr;
}

void ShadyCamera::destroy() {
	this->gridManager = nullptr;
}

void ShadyCamera::setGridManager(GridManager* newManager) {
	this->gridManager = newManager;
	if(newManager == nullptr)
		return;
	// Orient the camera around the grid
	GameObject* go = (GameObject*)this->GetObject();
	Transform* camTransform = go->GetTransform();
	camTransform->SetPosition(0.0f, 28.0f, 0.0f);
	camTransform->LookAt(0.0f, 0.0f, 0.0f);
	
  
}