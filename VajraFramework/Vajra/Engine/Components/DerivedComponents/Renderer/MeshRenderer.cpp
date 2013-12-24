#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

MeshRenderer::MeshRenderer() : Renderer() {
	this->init();
}

MeshRenderer::MeshRenderer(Object* object_) : Renderer(object_) {
	this->init();
}

MeshRenderer::~MeshRenderer() {
	this->destroy();
}

void MeshRenderer::InitMesh(std::string urlOfMesh) {
	this->meshAsset = ENGINE->GetAssetLibrary()->GetAsset<MeshAsset>(urlOfMesh);
	this->SetShaderName(this->meshAsset->GetShaderName());

	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	ASSERT(gameObject != nullptr, "GameObject not null");
	Transform* transform = gameObject->GetTransform();
	transform->SetPosition(this->meshAsset->GetInitialPosition().x, this->meshAsset->GetInitialPosition().y, this->meshAsset->GetInitialPosition().z);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().x, XAXIS);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().y, YAXIS);
	transform->SetOrientation(this->meshAsset->GetInitialRotation().z, ZAXIS);
	transform->SetScale(this->meshAsset->GetInitialScale().x, this->meshAsset->GetInitialScale().y, this->meshAsset->GetInitialScale().z);

	if (this->meshAsset->GetArmatureFilePath() != "") {
		Armature* armature = this->GetObject()->AddComponent<Armature>();
		armature->InitArmature(this->meshAsset->GetArmatureFilePath());
	}

	// Now that we are renderable, add self to the render lists in the scene graph:
	ASSERT(gameObject->GetParentSceneGraph() != nullptr, "GameObject has been added to a scenegraph");
	gameObject->GetParentSceneGraph()->AddGameObjectToRenderLists(gameObject);
}

void MeshRenderer::HandleMessage(Message* /* message */) {
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
