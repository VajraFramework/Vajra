#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/GameObject/GameObject.h"

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

void MeshRenderer::InitMesh(Mesh* newMesh) {
	this->mesh = newMesh;
}

void MeshRenderer::Update() {
}

void MeshRenderer::Draw() {
	if (this->mesh != 0) {
		this->mesh->Draw();
	}
}

void MeshRenderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->mesh = nullptr;
}

void MeshRenderer::destroy() {
	if (this->mesh != 0) {
		delete this->mesh;
	}
}
