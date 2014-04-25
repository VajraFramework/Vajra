//
//  BreakablePot.cpp
//  Created by Matt Kaufmann on 02/22/14.
//

#include "ExampleGame/Components/GameScripts/Units/Obstacles/BreakablePot.h"
#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Triggers/TriggerLoot.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

unsigned char ConvertStringToLinkDir(std::string str) {
	if      (str == "NORTH"    )  { return LINK_NORTH;     }
	else if (str == "EAST"     )  { return LINK_EAST;      }
	else if (str == "SOUTH"    )  { return LINK_SOUTH;     }
	else if (str == "WEST"     )  { return LINK_WEST;      }
	else if (str == "NORTHEAST")  { return LINK_NORTHEAST; }
	else if (str == "SOUTHEAST")  { return LINK_SOUTHEAST; }
	else if (str == "SOUTHWEST")  { return LINK_SOUTHWEST; }
	else if (str == "NORTHWEST")  { return LINK_NORTHWEST; }
	return LINK_NONE;
}

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
	this->deathEffectObjId = OBJECT_ID_INVALID;

	this->potLootId = OBJECT_ID_INVALID;
	this->linkages = LINK_NONE;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT, this->GetTypeId(), false);

}

void BreakablePot::destroy() {

}

void BreakablePot::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);
}

bool BreakablePot::CanBeKilledBy(ObjectIdType id, glm::vec3 /*source*/) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(gObj != nullptr, "GameObject exists with id %d", id);
	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if (unit != nullptr) {
			// Was it the assassin who attacked me?
			if (unit->GetUnitType() == UNIT_TYPE_ASSASSIN) {
				// Pots are destroyed by the assassin's attack.
				return true;
			}
		}
	}
	return false;
}

void BreakablePot::start() {

}

void BreakablePot::end() {

}

void BreakablePot::update() {

}

void BreakablePot::Kill() {
	BaseUnit::Kill();
	this->activateDeathEffect();
	this->gameObjectRef->SetVisible(false);
	this->tryLoot();
	this->chainReaction();
}

void BreakablePot::generateDeathEffect() {
	GameObject* deathEffectObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + this->deathEffect, ENGINE->GetSceneGraph3D());
	this->deathEffectObjId = deathEffectObj->GetId();
}

void BreakablePot::activateDeathEffect() {
	GameObject* deathEffectObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->deathEffectObjId);
	if (deathEffectObj != nullptr) {
		// Move the effect to this object's position
		Transform* myTrans = this->gameObjectRef->GetTransform();
		Transform* effectTrans = deathEffectObj->GetTransform();
		effectTrans->SetPositionWorld(myTrans->GetPositionWorld());

		ParticleSystem* deathEffectParticleSystem = deathEffectObj->GetComponent<ParticleSystem>();
		VERIFY(deathEffectParticleSystem != nullptr, "Death effect prefab has a particle system on it");
		deathEffectParticleSystem->Play();
	}
}

void BreakablePot::SetHasPotLoot() {
	GameObject* lootObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "SD_Loot_Non_Standalone.prefab", ENGINE->GetSceneGraph3D());
	this->potLootId = lootObj->GetId();
	this->GetObject()->AddChild(this->potLootId);
	lootObj->SetVisible(false);
}

void BreakablePot::AddLinkage(std::string dirStr) {
	AddLinkage(ConvertStringToLinkDir(dirStr));
}

void BreakablePot::AddLinkage(unsigned char dir) {
	this->linkages = this->linkages | dir;
}

void BreakablePot::tryLoot() {
	if(this->potLootId != OBJECT_ID_INVALID) {
		GameObject* lootObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->potLootId);
		if(lootObj != nullptr) {
			TriggerLoot* lootComp = lootObj->GetComponent<TriggerLoot>();
			ASSERT(lootComp != nullptr, "Loot game object has a TriggerLoot comp");
			if(lootComp != nullptr) {
				lootComp->ForceLootGrab();
			}
		}
	}
}

void BreakablePot::chainReaction() {
	GridCell* myCell = this->gridNavRef->GetCurrentCell();
	if (myCell != nullptr) {
		int x = myCell->x;
		int z = myCell->z;
		int elevation = this->gridNavRef->GetElevation();

		if ((this->linkages & LINK_NORTH    ) != LINK_NONE) {
			this->checkChainAtCell(x  , z+1, elevation);
		}
		if ((this->linkages & LINK_EAST     ) != LINK_NONE) {
			this->checkChainAtCell(x+1, z  , elevation);
		}
		if ((this->linkages & LINK_SOUTH    ) != LINK_NONE) {
			this->checkChainAtCell(x  , z-1, elevation);
		}
		if ((this->linkages & LINK_WEST     ) != LINK_NONE) {
			this->checkChainAtCell(x-1, z  , elevation);
		}
		if ((this->linkages & LINK_NORTHEAST) != LINK_NONE) {
			this->checkChainAtCell(x+1, z+1, elevation);
		}
		if ((this->linkages & LINK_SOUTHEAST) != LINK_NONE) {
			this->checkChainAtCell(x+1, z-1, elevation);
		}
		if ((this->linkages & LINK_SOUTHWEST) != LINK_NONE) {
			this->checkChainAtCell(x-1, z-1, elevation);
		}
		if ((this->linkages & LINK_NORTHWEST) != LINK_NONE) {
			this->checkChainAtCell(x-1, z+1, elevation);
		}
	}
}

void BreakablePot::checkChainAtCell(int x, int z, int elevation) {
	GridCell* targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
	if (targetCell != nullptr) {
		ObjectIdType occId = targetCell->GetOccupantIdAtElevation(elevation);
		GameObject* occ = ENGINE->GetSceneGraph3D()->GetGameObjectById(occId);
		if (occ != nullptr) {
			BaseUnit* occUnit = occ->GetComponent<BaseUnit>();
			if (occUnit != nullptr) {
				if ((occUnit->GetUnitType() == this->unitType) && (occUnit->GetUnitState() == UNIT_STATE_ALIVE)) {
					((BreakablePot*)occUnit)->Kill();
				}
			}
		}
	}
}
