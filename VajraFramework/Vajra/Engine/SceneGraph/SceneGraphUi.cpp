#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/Ui/UiTouchManager/UiTouchManager.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

SceneGraphUi::SceneGraphUi() : SceneGraph() {
}

SceneGraphUi::~SceneGraphUi() {
	this->destroy();
}

void SceneGraphUi::update() {
	this->handleTouches();
}

void SceneGraphUi::draw() {
	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);
	VERIFY(mainCamera != nullptr, "mainCamera set");

	Camera* cameraComponent = mainCamera->GetComponent<Camera>();
	VERIFY(cameraComponent != nullptr, "mainCamera has Camera component");

	this->renderLists->Draw(cameraComponent, nullptr);
}

bool SceneGraphUi::TestTouchStart(Touch touch) {
	return this->uiTouchManager->TestTouchCapture(touch);
}

void SceneGraphUi::OnTouchUpdate(int touchIndex) {
	this->uiTouchManager->OnTouchUpdate(touchIndex);
}

void SceneGraphUi::handleTouches() {
	if(ENGINE->GetInput()->GetTouchCount() != 1) {
		// Nothing to do

	} else {
		// Exactly one touch (not handling multi-touch yet)
		/// Touch touch = ENGINE->GetInput()->GetTouch(0);
		// float touchDisplacement = touch.pos.x - touch.prevPos.x;
		// glm::vec2 moveDir = touch.pos - touch.prevPos;
	}
}


void SceneGraphUi::init() {
	SceneGraph::init();
	//
	this->uiTouchManager = new UiTouchManager();
	this->mainCameraId = OBJECT_ID_INVALID;
}

void SceneGraphUi::destroy() {
	SceneGraph::destroy();
}
