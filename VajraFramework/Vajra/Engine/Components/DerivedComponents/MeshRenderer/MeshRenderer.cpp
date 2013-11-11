#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Utilities/MathUtilities.h"

unsigned int MeshRenderer::componentTypeId = COMPONENT_TYPE_ID_RENDERER;

MeshRenderer::MeshRenderer() : Component() {
	this->init();
}

MeshRenderer::MeshRenderer(Object* object_) : Component(object_) {
	this->init();
}

MeshRenderer::~MeshRenderer() {
	this->destroy();
}

void MeshRenderer::InitMesh(std::string urlOfMesh) {
	this->meshAsset = ENGINE->GetAssetLibrary()->GetAsset<MeshAsset>(urlOfMesh);

	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	ASSERT(gameObject != nullptr, "GameObject not null");
	Transform* transform = gameObject->GetTransform();
	transform->SetPosition(this->meshAsset->GetInitialPosition().x, this->meshAsset->GetInitialPosition().y, this->meshAsset->GetInitialPosition().z);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().x, XAXIS);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().y, YAXIS);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().z, ZAXIS);
	transform->SetScale(this->meshAsset->GetInitialScale().x, this->meshAsset->GetInitialScale().y, this->meshAsset->GetInitialScale().z);
}

void MeshRenderer::HandleMessage(Message* message) {
}

void MeshRenderer::Draw() {
	if (this->meshAsset) {
		this->meshAsset->Draw();
	}
}

void MeshRenderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}
}

void MeshRenderer::destroy() {
}
