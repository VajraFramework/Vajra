//
//  BreakablePot.cpp
//  Created by Matt Kaufmann on 02/22/14.
//

#include "ExampleGame/Components/GameScripts/Units/Obstacles/BreakablePot.h"
#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

BreakablePot::BreakablePot() : BaseUnit() {
	this->init();
}

BreakablePot::BreakablePot(Object* object_) : BaseUnit(object_) {
	this->init();
}

BreakablePot::~BreakablePot() {
	this->destroy();
}

void BreakablePot::init() {
	this->unitType = UNIT_TYPE_OBSTACLE;
	this->deathEffect = "";

	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT, this->GetTypeId(), false);
}

void BreakablePot::destroy() {

}

void BreakablePot::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_UNIT_SPECIAL_HIT:
			this->onUnitSpecialHit(messageChunk->GetSenderId(), messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			break;

		default:
			break;
	}
}

void BreakablePot::start() {

}

void BreakablePot::end() {

}

void BreakablePot::update() {

}

void BreakablePot::Kill() {
	BaseUnit::Kill();
	this->generateDeathEffect();
	this->gameObjectRef->SetVisible(false);
}

void BreakablePot::onUnitSpecialHit(ObjectIdType id, int gridX, int gridZ) {
	GridNavigator* gNav = this->GetObject()->GetComponent<GridNavigator>();
	if (gNav != nullptr) {
		// Did the special hit the cell I'm in?
		GridCell* cell = gNav->GetCurrentCell();
		if (cell != nullptr) {
			if ((cell->x == gridX) && (cell->z == gridZ)) {
				GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
				ASSERT(gObj != nullptr, "GameObject exists with id %d", id);
				if (gObj != nullptr) {
					BaseUnit* unit = gObj->GetComponent<BaseUnit>();
					if (unit != nullptr) {
						// Was it the assassin who attacked me?
						if (unit->GetUnitType() == UNIT_TYPE_ASSASSIN) {
							// Pots are destroyed by the assassin's attack.
							this->Kill();
						}
					}
				}
			}
		}
	}
}

void BreakablePot::generateDeathEffect() {
	GameObject* deathEffectObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + this->deathEffect, ENGINE->GetSceneGraph3D());
	if (deathEffectObj != nullptr) {
		// Move the effect to this object's position
		Transform* myTrans = this->gameObjectRef->GetTransform();
		Transform* effectTrans = deathEffectObj->GetTransform();
		effectTrans->SetPosition(myTrans->GetPositionWorld());
	}
}
