#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"

unsigned int MeshRenderer::componentTypeId = COMPONENT_TYPE_ID_RENDERER;

MeshRenderer::MeshRenderer() : Component() {
	this->init();
}

MeshRenderer::MeshRenderer(GameObject* gameObject_) : Component(gameObject_) {
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
	this->mesh = 0;
}

void MeshRenderer::destroy() {
	if (this->mesh != 0) {
		delete this->mesh;
	}
}
