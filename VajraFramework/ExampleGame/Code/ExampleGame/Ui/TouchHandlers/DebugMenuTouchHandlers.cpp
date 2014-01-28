#ifdef DEBUG
#include "ExampleGame/Ui/TouchHandlers/DebugMenuTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

void DebugMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch touch ) {
	if (uiObject->GetUiObjectName() == "touchPad") {
		// Do something

	} else {
		// Do something

	}
}

void DebugMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch ) {
	if (uiObject->GetUiObjectName() == "touchPad") {
		// Do something
		glm::vec2 moveDir = touch.pos - touch.prevPos;
		GameObject* debugCam = (GameObject*)ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject();
		Transform* trans = debugCam->GetTransform();

		trans->Rotate(-moveDir.x inRadians, YAXIS);
		trans->Rotate(moveDir.y inRadians, trans->GetLeft());

	} else {
		// Do something

	}
}

void DebugMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
	GameObject* debugCam = (GameObject*)ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject();
	Transform* trans = debugCam->GetTransform();
	if (uiObject->GetUiObjectName() == "up") {
		trans->Translate(this->cameraSpeed, trans->GetForward());
	} else if (uiObject->GetUiObjectName() == "down") {
		trans->Translate(-this->cameraSpeed, trans->GetForward());
	} else if (uiObject->GetUiObjectName() == "left") {
		trans->Translate(this->cameraSpeed, trans->GetLeft());
	} else if (uiObject->GetUiObjectName() == "right") {
		trans->Translate(-this->cameraSpeed, trans->GetLeft());
	} 
}
#endif