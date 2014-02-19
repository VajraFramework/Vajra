#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
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

void Renderer::addToRenderList() {
	GameObject* gameObject = (GameObject*)this->GetObject();
	ASSERT(gameObject->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");
	ASSERT(gameObject != nullptr, "GameObject not null");
	ASSERT(gameObject->GetParentSceneGraph() != nullptr, "GameObject has been added to a scenegraph");
	gameObject->GetParentSceneGraph()->AddGameObjectToRenderLists(gameObject);
}

void Renderer::removeFromRenderList() {
	GameObject* gameObject = (GameObject*)this->GetObject();
	ASSERT(gameObject->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");
	ASSERT(gameObject != nullptr, "GameObject not null");
	ASSERT(gameObject->GetParentSceneGraph() != nullptr, "GameObject has been added to a scenegraph");
	gameObject->GetParentSceneGraph()->RemoveGameObjectFromRenderLiset(gameObject);

}

void Renderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->hasTransperancy = false;
}

void Renderer::destroy() {
	this->removeFromRenderList();
}

