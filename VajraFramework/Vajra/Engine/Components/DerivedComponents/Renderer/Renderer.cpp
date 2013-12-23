#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Utilities/MathUtilities.h"

unsigned int Renderer::componentTypeId = COMPONENT_TYPE_ID_RENDERER;

Renderer::Renderer() : Component() {
	this->init();
}

Renderer::Renderer(Object* object_) : Component(object_) {
	this->init();
}

Renderer::~Renderer() {
	this->destroy();
}

void Renderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}
}

void Renderer::destroy() {
}
