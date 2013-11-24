#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////

class RenderList {
public:
	RenderList(std::string shaderName_);
	void Draw();

	inline std::string GetShaderName() { return this->shaderName; }
	void AddGameObjectId(ObjectIdType id);
	void RemoveGameObjectId(ObjectIdType id);

private:
	std::string shaderName;
	std::vector<ObjectIdType> gameObjectIds;
};

RenderList::RenderList(std::string shaderName_) {
	this->shaderName = shaderName_;
}

void RenderList::AddGameObjectId(ObjectIdType id) {
	// TODO [Implement] Make sure there are no duplicates
	// TODO [Implement] Keep this sorted so that insertions w/o duplicates and deletions are faster
	this->gameObjectIds.push_back(id);
}

void RenderList::RemoveGameObjectId(ObjectIdType id) {
	// TODO [Implement] Make sure there are no duplicates
	// TODO [Implement] Keep this sorted so that insertions w/o duplicates and deletions are faster
	auto id_it = std::find(this->gameObjectIds.begin(), this->gameObjectIds.end(), id);
	if (id_it != this->gameObjectIds.end()) {
		this->gameObjectIds.erase(id_it);
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to remove game object id %d from renderlist %s. No such game object id", id, this->shaderName.c_str());
	}
}

void RenderList::Draw() {
	for (ObjectIdType id : this->gameObjectIds) {
		GameObject* gameObject = ENGINE->GetSceneGraph()->GetGameObjectById(id);
		if (gameObject != nullptr) {
			gameObject->Draw();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

RenderLists::RenderLists() {
	this->init();
}

RenderLists::~RenderLists() {
	this->destroy();
}

void RenderLists::Begin() {
	this->currentRenderListIdx = 0;
}

void RenderLists::Next() {
	this->currentRenderListIdx++;
}

bool RenderLists::PrepareCurrentRenderList() {
	if (this->currentRenderListIdx < this->renderLists.size()) {
		FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet(this->renderLists[this->currentRenderListIdx]->GetShaderName());
		return true;
	}
	return false;
}

void RenderLists::RenderGameObjectsInCurrentList() {
	ASSERT(this->currentRenderListIdx < this->renderLists.size(), "\nCurrent render list idx is valid");
	this->renderLists[this->currentRenderListIdx]->Draw();
}

void RenderLists::init() {
	this->currentRenderListIdx = 0;
	this->createRenderLists();
}

void RenderLists::destroy() {
	for (unsigned int i = 0; i < this->renderLists.size(); ++i) {
		RenderList* renderList = this->renderLists[i];
		delete renderList;
	}
	this->renderLists.clear();
}

void RenderLists::addGameObjectIdToRenderList(ObjectIdType id, std::string shaderName) {
	for (RenderList* renderList : this->renderLists) {
		if (renderList->GetShaderName() == shaderName) {
			renderList->AddGameObjectId(id);
			return;
		}
	}
	FRAMEWORK->GetLogger()->dbglog("\nWARNING: Trying to add game object id %d to render list %s. No such shader render list", id, shaderName.c_str());
}

void RenderLists::createRenderLists() {
	ASSERT_LOG(this->renderLists.size() == 0, "Creating RenderLists for the first time");

	std::vector<std::string> shaderNames;
	FRAMEWORK->GetOpenGLWrapper()->GetAllAvailableShaderNames(shaderNames);

	for (std::string shaderName : shaderNames) {
		RenderList* newRenderList = new RenderList(shaderName);
		this->renderLists.push_back(newRenderList);
	}

	FRAMEWORK->GetLogger()->dbglog("\nCreated %d render lists", this->renderLists.size());
}
