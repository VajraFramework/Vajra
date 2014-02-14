#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitShadow.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

ComponentIdType UnitShadow::componentTypeId = COMPONENT_TYPE_ID_UNIT_SHADOW;

UnitShadow::UnitShadow() : Component() {
	this->init();
}

UnitShadow::UnitShadow(Object* object_) : Component(object_) {
	this->init();
}

UnitShadow::~UnitShadow() {
	this->destroy();
}

void UnitShadow::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
}

void UnitShadow::start() {
	// TODO [Implement] Ensure type safety here
	this->gameObjectRef = (GameObject*)this->GetObject();

	// Create a little shadow prefab and child it to the parent game object:
	this->littleShadow = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->littleShadow->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "littleShadow.png");
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
	this->gameObjectRef->AddChild(this->littleShadow->GetId());
	glm::vec3 parentScale = this->gameObjectRef->GetTransform()->GetScale();
	this->littleShadow->GetTransform()->Scale(1.0f / parentScale.x, 1.0f / parentScale.y, 1.0f / parentScale.z);
	this->littleShadow->GetTransform()->Translate(0.1f, YAXIS);
	this->littleShadow->GetTransform()->Rotate(90.0f inRadians, XAXIS);
}

void UnitShadow::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);

	// TODO [Hack] Remove this when we have level start and end messages being sent
	this->start();
}

void UnitShadow::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
