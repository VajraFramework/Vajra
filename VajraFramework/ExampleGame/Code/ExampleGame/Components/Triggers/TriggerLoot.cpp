#include "ExampleGame/Components/Triggers/TriggerLoot.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/MathUtilities.h"

// Tween callbacks
void lootNumberTweenCallback(float /*fromNumber*/, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	if(go != nullptr) {
		TriggerLoot* loot = go->GetComponent<TriggerLoot>();
		if(loot != nullptr) {
			float yOffset = cos(currentNumber) * 1.5f;
			go->GetTransform()->Translate(0.0f, loot->startPos.y + yOffset, 0.0f);
			if(currentNumber == toNumber) {
				loot->lootTweenEnd();
			}
		}
	}
}
TriggerLoot::TriggerLoot() : Triggerable() {
	this->init();
}

TriggerLoot::TriggerLoot(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerLoot::~TriggerLoot() {
	this->destroy();
}

void TriggerLoot::init() {
	// create the partile effect
	GameObject* particleEffect = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "lootBurst.prefab", ENGINE->GetSceneGraph3D());
	this->particleEffectId = particleEffect->GetId();
}

void TriggerLoot::destroy() {

}

void TriggerLoot::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);
}

void TriggerLoot::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerLoot::SetMoneyValue(int v) {
	this->moneyValue = v;
}

void TriggerLoot::onSwitchToggled(bool switchState) {
	this->startPos = this->GetObject()->GetComponent<Transform>()->GetPosition();
	// Perform a position tween
	MessageData1S1I1F* params = new MessageData1S1I1F();
	params->i = this->GetObject()->GetId();
	ENGINE->GetTween()->TweenToNumber(0.0f, 1.8f, .3f, INTERPOLATION_TYPE_LINEAR, true, false, true, "lootBounce" + params->i, NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, params, lootNumberTweenCallback);
}

void TriggerLoot::lootTweenEnd() {
	// TODO add in number tween and particle burst
	GameObject* particleEffectObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->particleEffectId);
	if (particleEffectObj != nullptr) {
		// Move the effect to this object's position
		Transform* myTrans = this->GetObject()->GetComponent<Transform>();
		Transform* effectTrans = particleEffectObj->GetTransform();
		effectTrans->SetPositionWorld(myTrans->GetPositionWorld());

		ParticleSystem* particleSystem = particleEffectObj->GetComponent<ParticleSystem>();
		VERIFY(particleSystem != nullptr, "particle effect prefab has a particle system on it");
		particleSystem->Play();
	}
	// Hide the mesh
	((GameObject*)this->GetObject())->SetVisible(false);

	// Send a message that the loot was collected
	SINGLETONS->GetMasteryManager()->AddMoney(this->moneyValue);
}
