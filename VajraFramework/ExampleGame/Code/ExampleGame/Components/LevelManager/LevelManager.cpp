//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/LevelManager/MasteryManager.h"
#include "ExampleGame/Components/Triggers/TriggerLevelDefeat.h"
#include "ExampleGame/Components/Triggers/TriggerLevelVictory.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/SavedDataManager/SavedDataManager.h"
#include "Vajra/Utilities/StringUtilities.h"


#include <fstream>

ComponentIdType LevelManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

LevelManager::LevelManager() : Component() {
	this->init();
}

LevelManager::LevelManager(Object* object_) : Component(object_) {
	this->init();
}

LevelManager::~LevelManager() {
	this->destroy();
}

void LevelManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
			break;
		case MESSAGE_TYPE_ON_END_CONDITIONS_MET:
			break;
		case MESSAGE_TYPE_LEVEL_UNLOADED:
			if(this->levelToLoad != -1) {
				MessageChunk mc = ENGINE->GetMessageHub()->GetOneFreeMessage();
				mc->SetMessageType(MESSAGE_TYPE_LOAD_LEVEL);
				// TODO [HACK We need this delay when loading a new level
				mc->messageData.iv1.x = 3;
				ENGINE->GetMessageHub()->SendPointcastMessage(mc, this->GetObject()->GetId(), this->GetObject()->GetId());
			}
			break;
		case MESSAGE_TYPE_LOAD_LEVEL:
				if(messageChunk->messageData.iv1.x == 0) {
					this->loadLevel_internal();
				} else {
					MessageChunk mc = ENGINE->GetMessageHub()->GetOneFreeMessage();
					mc->SetMessageType(MESSAGE_TYPE_LOAD_LEVEL);
					mc->messageData.iv1.x = messageChunk->messageData.iv1.x - 1;
					ENGINE->GetMessageHub()->SendPointcastMessage(mc, this->GetObject()->GetId(), this->GetObject()->GetId());
				}
			break;
		default:
			break;
	}
}

void LevelManager::LoadLevel(int levelNumber) {
	this->levelToLoad = levelNumber;
	this->UnloadLevel();
}

void LevelManager::UnloadLevel() {
	// Unload the previous scene and all other items in the SceneGraph3D
	this->clearEndConditions();
	ENGINE->GetSceneGraph3D()->UnloadCurrentScene();
	ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LEVEL_UNLOADED);
}

void LevelManager::ReloadCurrentLevel() {
	this->LoadLevel(this->currentLevelIndex);
}

bool LevelManager::TryLoadNextLevel() {
	if(this->currentLevelIndex + 1 < this->GetNumLevelsInCurrentMission()) {
		this->LoadLevel(this->currentLevelIndex + 1);
		return true;
	}
	return false;
}

void LevelManager::StartLevel() {
	ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_SCENE_START);
}

void LevelManager::loadLevel_internal() {
	ASSERT(this->levelToLoad < this->GetNumLevelsInCurrentMission(), "level number is less than the number of levels");
	if(this->levelToLoad < this->GetNumLevelsInCurrentMission()) {
		this->currentLevelIndex = this->levelToLoad;
		this->LoadLevelFromData(this->GetLevelData(this->currentMission, this->levelToLoad));
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LEVEL_LOADED);
	}
	this->levelToLoad = -1;
}

void LevelManager::LoadLevelFromData(LevelData* levelData) {
	LevelLoader::LoadLevelFromFile(levelData->path);
	if(levelData->hasTutorial) {
		LevelLoader::LoadTutorialData(levelData->path);
	}
	// Set up the bonus
	SINGLETONS->GetMasteryManager()->SetCurrentBonus(levelData->bonus, levelData->bonusValue, this->GetCurrentLevelIndex(), levelData->bounty);
}

LevelData* LevelManager::GetLevelData(int missionIndex, int levelIndex) 
{ 
	return this->GetLevelData(this->currentContract, missionIndex, levelIndex);
}

LevelData* LevelManager::GetLevelData(int contractIndex, int missionIndex, int levelIndex) 
{ 
	std::vector<LevelData*> lData = this->GetMissionData(contractIndex, missionIndex)->levels;
	if(levelIndex < (int)lData.size()) {
		return lData[levelIndex];
	}
	ASSERT(false, "index passed into GetLevelData is valid");
	return lData[0];
}

MissionData* LevelManager::GetMissionData(int contractIndex, int missionIndex) 
{ 
	std::vector<MissionData*> mData = this->GetContractData(contractIndex)->missions;
	if(missionIndex < (int)mData.size()) {
		return mData[missionIndex];
	}
	ASSERT(false, "missionIndex passed into GetMissionData is valid");
	return mData[0];
}
ContractData* LevelManager::GetContractData(int index) 
{ 
	if(index < (int)this->contractData.size()) {
		return this->contractData[index];
	}
	ASSERT(false, "index passed into GetContractData is valid");
	return this->contractData[0];
}

int LevelManager::GetNumLevelsInMission(int mission) {
	MissionData* mData = GetMissionData(this->currentContract, mission);
	return mData->levels.size();
}

void LevelManager::OnCurrentLevelWon(LevelCompletion completion) {
	LevelData* lData = this->GetLevelData(this->currentMission, this->currentLevelIndex);
	if(completion > lData->completion) {
		Bundle* bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle(PLAYER_BUNDLE_NAME);

		lData->completion = completion;
		
		if(lData->completion == LevelCompletion::Completed) {
			this->levelCompletionData[lData->levelNum] = 'C';
		} else if (lData->completion == LevelCompletion::Completed_Bonus) {
			this->levelCompletionData[lData->levelNum] = 'B';

		}
		
		if(lData->name != "SD_Prison_2") {
			LevelData* nextLevel = this->getNextLevel();
			if(nextLevel != nullptr && nextLevel->completion == LevelCompletion::Locked) {
				nextLevel->completion = LevelCompletion::Unlocked;
				this->levelCompletionData[nextLevel->levelNum] = 'U';
				this->onLevelUnlocked(nextLevel->levelNum);
			}
		}
		bundle->PutString(LEVEL_COMPLETION, this->levelCompletionData);
		bundle->Save();
	}
}

LevelData* LevelManager::getNextLevel() {
	int lIndex = this->currentLevelIndex + 1;
	int mIndex = this->currentMission;
	int cIndex = this->currentContract;
	if(lIndex < this->GetNumLevelsInCurrentMission()) {
		return this->GetLevelData(mIndex, lIndex);
	} 
	mIndex++;
	if(mIndex < GetNumMissionsInCurrentContract()) {
		return this->GetLevelData(mIndex, 0);
	}
	cIndex++;
	if(cIndex < this->NumContracts()) {
		return this->GetLevelData(cIndex, mIndex, lIndex);
	}
	// this is the last level
	return nullptr;
}

void LevelManager::AddWinCondition(ObjectIdType switchId) {
	Triggerable* victoryTrigger = this->winner->GetComponent<Triggerable>();
	victoryTrigger->SubscribeToSwitchObject(switchId);
}

void LevelManager::AddLoseCondition(ObjectIdType switchId) {
	Triggerable* defeatTrigger = this->loser->GetComponent<Triggerable>();
	defeatTrigger->SubscribeToSwitchObject(switchId);
}

void LevelManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LEVEL_UNLOADED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LOAD_LEVEL, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ON_END_CONDITIONS_MET, this->GetTypeId(), false);

	// Generate required end condition objects
	this->winner = new Object();
	winner->AddComponent<TriggerLevelVictory>();

	this->loser = new Object();
	loser->AddComponent<TriggerLevelDefeat>();

	// load player data
	Bundle* bundle = nullptr;
	if (!FRAMEWORK->GetSavedDataManager()->HasBundle(PLAYER_BUNDLE_NAME)) {
		this->initBundleForFirstTime();
	} 
	
	bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle(PLAYER_BUNDLE_NAME);
	this->levelCompletionData = bundle->GetString(LEVEL_COMPLETION);
	for(int i = 0; i < (int)levelCompletionData.size(); ++i) {
		char levelStatus = levelCompletionData[i];
		if(levelStatus != 'L') {
			this->loadLevelScore(i);
		} else {
			break;
		}
	}


	this->levelToLoad = -1;
	currentLevelIndex = 0;
	currentMission = 0;
	currentContract = 0;
	// load the list of levels with a tutorial
	LevelLoader::LoadLevelData(&this->contractData);
}

void LevelManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	if (this->winner != nullptr) {
		delete this->winner;
		this->winner = nullptr;
	}

	if (this->loser != nullptr) {
		delete this->loser;
		this->loser = nullptr;
	}
}

void LevelManager::update() {
}

void LevelManager::endLevel(bool /*success*/) {
	//LevelEnd.EndLevel(success);
}

void LevelManager::clearEndConditions() {
	Triggerable* victoryTrigger = winner->GetComponent<TriggerLevelVictory>();
	if (victoryTrigger != nullptr) {
		victoryTrigger->UnsubscribeToAllSwitches();
	}

	Triggerable* defeatTrigger = loser->GetComponent<TriggerLevelDefeat>();
	if (defeatTrigger != nullptr) {
		defeatTrigger->UnsubscribeToAllSwitches();
	}
}

void LevelManager::initBundleForFirstTime() {
	Bundle* bundle = FRAMEWORK->GetSavedDataManager()->CreateNewBundle(PLAYER_BUNDLE_NAME);
	
	// create level completion data
	char levelCompletion[MAX_LEVELS_POSSIBLE+1];
	levelCompletion[0] = 'U'; // U = unlocked
	this->onLevelUnlocked(0);
	for(int i = 1; i < MAX_LEVELS_POSSIBLE; ++i) {
#ifdef DEBUG
		levelCompletion[i] = 'U';
		this->onLevelUnlocked(i);
#else
		levelCompletion[i] = 'L'; // L = locked
#endif
	}
	levelCompletion[MAX_LEVELS_POSSIBLE] = '\0';

	std::string levelData = levelCompletion;
	bundle->PutString(LEVEL_COMPLETION, levelData);

	bundle->Save();
}

void LevelManager::onLevelUnlocked(int index) {
	LevelScores scores;
	scores.bonus = false;
	scores.take = 0;
	scores.time = -1;
	scores.kills = -1;
	scores.alerts = -1;
	scores.money = -1;
	this->SaveLevelScores(index, &scores);
	SINGLETONS->GetMasteryManager()->updateLevelScore(index, scores);

}

void LevelManager::loadLevelScore(int index) {
	std::string bundleName = LEVEL_BUNDLE_NAME + StringUtilities::ConvertIntToString(index);
	Bundle* bundle;
 	LevelScores scores;

	if(!FRAMEWORK->GetSavedDataManager()->HasBundle(bundleName)) {
		bundle = FRAMEWORK->GetSavedDataManager()->CreateNewBundle(bundleName);
	} else {
		bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle(bundleName);
	}
 
	scores.bonus = bundle->GetBool(BONUS);
	scores.take = bundle->GetInt(BEST_TAKE);
	scores.time = bundle->GetInt(BEST_TIME);
	scores.kills = bundle->GetInt(BEST_KILL);
	scores.alerts = bundle->GetInt(BEST_ALERT);
	scores.money = bundle->GetInt(BEST_LOOT);

	bundle->Save();
	SINGLETONS->GetMasteryManager()->updateLevelScore(index, scores);

}

void LevelManager::SaveLevelScores(int levelIndex, LevelScores* scores) {
	std::string bundleName = LEVEL_BUNDLE_NAME + StringUtilities::ConvertIntToString(levelIndex);
	Bundle* bundle;

	if(!FRAMEWORK->GetSavedDataManager()->HasBundle(bundleName)) {
		bundle = FRAMEWORK->GetSavedDataManager()->CreateNewBundle(bundleName);
	} else {
		bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle(bundleName);
	}

	bundle->PutBool(BONUS, scores->bonus);
	bundle->PutInt(BEST_TAKE, scores->take);
	bundle->PutInt(BEST_TIME, scores->time);
	bundle->PutInt(BEST_KILL, scores->kills);
	bundle->PutInt(BEST_ALERT, scores->alerts);
	bundle->PutInt(BEST_LOOT, scores->money);

	bundle->Save();

}
