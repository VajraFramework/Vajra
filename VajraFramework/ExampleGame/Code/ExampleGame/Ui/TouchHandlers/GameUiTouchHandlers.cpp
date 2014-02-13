#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"


#if DEBUG
#include "ExampleGame/Ui/TouchHandlers/DebugMenuTouchHandlers.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#endif

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#define IN_GAME_MENU "inGame"
#define PAUSE_MENU "pauseMenu"
#define POST_GAME_MENU "postGame"
#define TURORIAL_MENU "tutorialScreen"

#define ASSASSIN_ICON_INDEX 0
#define THIEF_ICON_INDEX 1

void onTutorialTweenInComplete(ObjectIdType gameObjectId, std::string tweenClipName) {
	printf("CLIP NAME %s", tweenClipName.c_str());

	UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is null in onTutorialTweenInComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(true);
	}

}
void onTutorialTweenOutComplete(ObjectIdType gameObjectId, std::string tweenClipName) {
	printf("CLIP NAME %s", tweenClipName.c_str());
	
	UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is null in onTutorialTweenOutComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(false);
	}
	
}
GameUiTouchHandlers::GameUiTouchHandlers() : UiTouchHandlers() {
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
}

void GameUiTouchHandlers::HandleMessageCallback(MessageChunk messageChunk) {
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SELECTED_UNIT_CHANGED:
			if(messageChunk->messageData.iv1.y == UNIT_TYPE_ASSASSIN) {
				// TODO [Implement] Ensure type safety here
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				changeUnitIcon->SetSpriteTextureIndex(ASSASSIN_ICON_INDEX);

			} else if(messageChunk->messageData.iv1.y == UNIT_TYPE_THIEF) {
				// TODO [Implement] Ensure type safety here
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				changeUnitIcon->SetSpriteTextureIndex(THIEF_ICON_INDEX);
			}
			break;
		default:
			break;
	}
	UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TURORIAL_MENU]);
	tut->SetClickable(false);
	ENGINE->GetTween()->TweenPosition(tut->GetId(),
									  tut->GetTransform()->GetPosition(),
									  glm::vec3(tut->GetTransform()->GetPosition().x, 0.0f, tut->GetTransform()->GetPosition().z),
									  1.0f,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
									  false,
									  onTutorialTweenInComplete);

}

void GameUiTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch /* touch */) {

	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
#ifdef DEBUG 
	if (uiObject->GetName() == "debugMenu") {
		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "debugMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new DebugMenuTouchHandlers());
		// Return immediately here because the current scene has been unloaded
		return;
	}
#endif
	if (uiObject->GetName() == "pause") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(!pauseMenu->IsVisible());
	} else if (uiObject->GetName() == "resume") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);
	} else if (uiObject->GetName() == "mission_from_pause") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);

		//ENGINE->GetSceneGraph3D()->UnloadCurrentScene();

		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "mainMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new MainMenuTouchHandlers());

	} else if(uiObject->GetName() == "changeUnit") {
		SINGLETONS->GetGridManager()->SwitchSelectedUnit();

	} else if(uiObject->GetName() == "closeTutorial") {
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TURORIAL_MENU]);
		tut->SetClickable(false);
		ENGINE->GetTween()->TweenPosition(tut->GetId(),
										  tut->GetTransform()->GetPosition(),
										  glm::vec3(tut->GetTransform()->GetPosition().x, -768.0f, tut->GetTransform()->GetPosition().z),
										  1.0f,
										  false,
										  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
										  false,
										  onTutorialTweenInComplete);

	}
}
