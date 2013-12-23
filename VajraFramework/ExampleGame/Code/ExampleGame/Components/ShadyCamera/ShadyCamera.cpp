#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Utilities/MathUtilities.h"
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
	Transform* camTransform = ((GameObject*)this->GetObject())->GetTransform();
	camTransform->SetPosition(0.0f, 25.0f, 0.0f);
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