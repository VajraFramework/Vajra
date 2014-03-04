#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Decals/Decal.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

ComponentIdType Decal::componentTypeId = COMPONENT_TYPE_ID_DECAL;

#define GAME_MODE_IMAGE_IDX 0
#define OVERVIEW_MODE_IMAGE_IDX 1

Decal::Decal() : Component() {
	this->init();
}

Decal::Decal(Object* object_) : Component(object_) {
	this->init();
}

Decal::~Decal() {
	this->destroy();
}

void Decal::SetDecalImages(std::string gameMode_image, float gameMode_imageSize, std::string overviewMode_image, float overviewMode_imageSize) {
	// Create a game object to hold the decals:
	this->decalObjectRef = new GameObject(this->gameObjectRef->GetParentSceneGraph());

	// Add a sprite renderer:
	SpriteRenderer* spriteRenderer = this->decalObjectRef->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	if (gameMode_image     == "") { gameMode_image     = "blank.png"; }
	if (overviewMode_image == "") { overviewMode_image = "blank.png"; }
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + gameMode_image);
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + overviewMode_image);
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
	spriteRenderer->SetCurrentTextureIndex(GAME_MODE_IMAGE_IDX);

	this->scales[GAME_MODE_IMAGE_IDX] = gameMode_imageSize;
	this->scales[OVERVIEW_MODE_IMAGE_IDX] = overviewMode_imageSize;

	this->decalObjectRef->GetTransform()->Rotate(-90.0f inRadians, XAXIS);
	this->decalObjectRef->GetTransform()->SetScale(this->scales[this->currentMode], this->scales[this->currentMode], this->scales[this->currentMode]);

	this->updatePosition();
}

void Decal::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
	case MESSAGE_TYPE_CAMERA_MODE_CHANGED: {
		this->handleCameraModeChanged();
	} break;

	default: {
	} break;
	}
}

void Decal::update() {
	this->updatePosition();
}

void Decal::updatePosition() {
	if (this->decalObjectRef != nullptr) {
		this->decalObjectRef->GetTransform()->SetPosition(this->gameObjectRef->GetTransform()->GetPositionWorld());
		this->decalObjectRef->GetTransform()->Translate(2.0f, YAXIS);
	}
}

void Decal::handleCameraModeChanged() {
	if (!this->decalObjectRef) {
		return;
	}
	SpriteRenderer* spriteRenderer = this->decalObjectRef->GetComponent<SpriteRenderer>();
	if (!spriteRenderer) {
		return;
	}

	ShadyCamera* shadyCamera = SINGLETONS->GetGridManager()->GetShadyCamera();
	ASSERT(shadyCamera != nullptr, "Got shady camera");
	if (shadyCamera != nullptr) {
		if (shadyCamera->GetCameraMode() == ShadyCamera::CameraMode_Game) {
			this->currentMode = GAME_MODE_IMAGE_IDX;
			spriteRenderer->SetIsOverlay(false);

		} else if (shadyCamera->GetCameraMode() == ShadyCamera::CameraMode_Overview) {
			this->currentMode = OVERVIEW_MODE_IMAGE_IDX;
			spriteRenderer->SetIsOverlay(true);

		}

		spriteRenderer->SetCurrentTextureIndex(this->currentMode);
		this->decalObjectRef->GetTransform()->SetScale(this->scales[this->currentMode], this->scales[this->currentMode], this->scales[this->currentMode]);
	}
}

void Decal::init() {
	this->decalObjectRef = nullptr;

	this->currentMode = GAME_MODE_IMAGE_IDX;

	this->scales[GAME_MODE_IMAGE_IDX] = 1.0f;
	this->scales[OVERVIEW_MODE_IMAGE_IDX] = 1.0f;

	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->addSubscriptionToMessageType(MESSAGE_TYPE_CAMERA_MODE_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void Decal::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

