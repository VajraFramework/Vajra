#include "ExampleGame/ComponentMapper/ComponentMapper.h"
#include "Vajra/Utilities/StringUtilities.h"

#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"


Component* ComponentMapper::AddNewComponentToGameObjectByComponentName(GameObject* gameObject, std::string componentName) {

	
	if (componentName == "RigidAnimation") {
		RigidAnimation* component = gameObject->GetComponent<RigidAnimation>();
		if (component == nullptr) { component = gameObject->AddComponent<RigidAnimation>(); }
		return component;
	}
	
	if (componentName == "BakedSkeletalAnimation") {
		BakedSkeletalAnimation* component = gameObject->GetComponent<BakedSkeletalAnimation>();
		if (component == nullptr) { component = gameObject->AddComponent<BakedSkeletalAnimation>(); }
		return component;
	}
	
	if (componentName == "Camera") {
		Camera* component = gameObject->GetComponent<Camera>();
		if (component == nullptr) { component = gameObject->AddComponent<Camera>(); }
		return component;
	}
	
	if (componentName == "DirectionalLight") {
		DirectionalLight* component = gameObject->GetComponent<DirectionalLight>();
		if (component == nullptr) { component = gameObject->AddComponent<DirectionalLight>(); }
		return component;
	}
	
	if (componentName == "MeshRenderer") {
		MeshRenderer* component = gameObject->GetComponent<MeshRenderer>();
		if (component == nullptr) { component = gameObject->AddComponent<MeshRenderer>(); }
		return component;
	}
	
	if (componentName == "AudioSource") {
		AudioSource* component = gameObject->GetComponent<AudioSource>();
		if (component == nullptr) { component = gameObject->AddComponent<AudioSource>(); }
		return component;
	}
	
	if (componentName == "Armature") {
		Armature* component = gameObject->GetComponent<Armature>();
		if (component == nullptr) { component = gameObject->AddComponent<Armature>(); }
		return component;
	}
	
	if (componentName == "Transform") {
		Transform* component = gameObject->GetComponent<Transform>();
		if (component == nullptr) { component = gameObject->AddComponent<Transform>(); }
		return component;
	}
	
	if (componentName == "LevelManager") {
		LevelManager* component = gameObject->GetComponent<LevelManager>();
		if (component == nullptr) { component = gameObject->AddComponent<LevelManager>(); }
		return component;
	}
	
	if (componentName == "SampleGameScript") {
		SampleGameScript* component = gameObject->GetComponent<SampleGameScript>();
		if (component == nullptr) { component = gameObject->AddComponent<SampleGameScript>(); }
		return component;
	}
	
	if (componentName == "AiPerception") {
		AiPerception* component = gameObject->GetComponent<AiPerception>();
		if (component == nullptr) { component = gameObject->AddComponent<AiPerception>(); }
		return component;
	}
	
	if (componentName == "AiRoutine") {
		AiRoutine* component = gameObject->GetComponent<AiRoutine>();
		if (component == nullptr) { component = gameObject->AddComponent<AiRoutine>(); }
		return component;
	}
	
	if (componentName == "AiKnowledge") {
		AiKnowledge* component = gameObject->GetComponent<AiKnowledge>();
		if (component == nullptr) { component = gameObject->AddComponent<AiKnowledge>(); }
		return component;
	}
	
	if (componentName == "EnemyUnit") {
		EnemyUnit* component = gameObject->GetComponent<EnemyUnit>();
		if (component == nullptr) { component = gameObject->AddComponent<EnemyUnit>(); }
		return component;
	}
	
	if (componentName == "PlayerUnit") {
		PlayerUnit* component = gameObject->GetComponent<PlayerUnit>();
		if (component == nullptr) { component = gameObject->AddComponent<PlayerUnit>(); }
		return component;
	}
	
	if (componentName == "ShadyCamera") {
		ShadyCamera* component = gameObject->GetComponent<ShadyCamera>();
		if (component == nullptr) { component = gameObject->AddComponent<ShadyCamera>(); }
		return component;
	}
	
	if (componentName == "GridManager") {
		GridManager* component = gameObject->GetComponent<GridManager>();
		if (component == nullptr) { component = gameObject->AddComponent<GridManager>(); }
		return component;
	}
	
	if (componentName == "GridZone") {
		GridZone* component = gameObject->GetComponent<GridZone>();
		if (component == nullptr) { component = gameObject->AddComponent<GridZone>(); }
		return component;
	}
	
	if (componentName == "GridNavigator") {
		GridNavigator* component = gameObject->GetComponent<GridNavigator>();
		if (component == nullptr) { component = gameObject->AddComponent<GridNavigator>(); }
		return component;
	}

	return nullptr;
}


void ComponentMapper::InitializePropertyByComponentAndPropertyNames(GameObject *gameObject, std::string componentName, std::string propertyName, std::vector<std::string> argv) {

	
	if (componentName == "RigidAnimation") {
		RigidAnimation* component = gameObject->GetComponent<RigidAnimation>();
		if (component == nullptr) { return; }
		if (propertyName == "AddAnimationClip") {
			if ((int)argv.size() < 1) { return; }
			component->AddAnimationClip(ConvertStringToString(argv[0]));
		}
	}
	
	if (componentName == "BakedSkeletalAnimation") {
		BakedSkeletalAnimation* component = gameObject->GetComponent<BakedSkeletalAnimation>();
		if (component == nullptr) { return; }
		if (propertyName == "AddAnimationClip") {
			if ((int)argv.size() < 1) { return; }
			component->AddAnimationClip(ConvertStringToString(argv[0]));
		}
	}
	
	if (componentName == "Camera") {
		Camera* component = gameObject->GetComponent<Camera>();
		if (component == nullptr) { return; }
		if (propertyName == "SetCameraType") {
			if ((int)argv.size() < 1) { return; }
			component->SetCameraType(StringUtilities::ConvertStringToInt(argv[0]));
		}
		if (propertyName == "SetFOV") {
			if ((int)argv.size() < 1) { return; }
			component->SetFOV(StringUtilities::ConvertStringToFloat(argv[0]));
		}
	}
	
	if (componentName == "DirectionalLight") {
		DirectionalLight* component = gameObject->GetComponent<DirectionalLight>();
		if (component == nullptr) { return; }
		if (propertyName == "SetAmbientColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetAmbientColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
		}
		if (propertyName == "SetDiffuseColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetDiffuseColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
		}
		if (propertyName == "SetSpecularColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetSpecularColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
		}
	}
	
	if (componentName == "MeshRenderer") {
		MeshRenderer* component = gameObject->GetComponent<MeshRenderer>();
		if (component == nullptr) { return; }
		if (propertyName == "InitMesh") {
			if ((int)argv.size() < 1) { return; }
			component->InitMesh(ConvertStringToString(argv[0]));
		}
	}
	
	if (componentName == "AudioSource") {
		AudioSource* component = gameObject->GetComponent<AudioSource>();
		if (component == nullptr) { return; }
		if (propertyName == "SetAudioClip") {
			if ((int)argv.size() < 1) { return; }
			component->SetAudioClip(ConvertStringToString(argv[0]));
		}
		if (propertyName == "SetVolume") {
			if ((int)argv.size() < 1) { return; }
			component->SetVolume(StringUtilities::ConvertStringToFloat(argv[0]));
		}
		if (propertyName == "SetPlaybackSpeed") {
			if ((int)argv.size() < 1) { return; }
			component->SetPlaybackSpeed(StringUtilities::ConvertStringToFloat(argv[0]));
		}
	}
	
	if (componentName == "Armature") {
		Armature* component = gameObject->GetComponent<Armature>();
		if (component == nullptr) { return; }
		if (propertyName == "InitArmature") {
			if ((int)argv.size() < 1) { return; }
			component->InitArmature(ConvertStringToString(argv[0]));
		}
	}
	
	if (componentName == "Transform") {
		Transform* component = gameObject->GetComponent<Transform>();
		if (component == nullptr) { return; }
		if (propertyName == "SetPosition") {
			if ((int)argv.size() < 3) { return; }
			component->SetPosition(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
		}
		if (propertyName == "SetOrientation") {
			if ((int)argv.size() < 4) { return; }
			component->SetOrientation(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
		}
		if (propertyName == "SetScale") {
			if ((int)argv.size() < 3) { return; }
			component->SetScale(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
		}
		if (propertyName == "Translate") {
			if ((int)argv.size() < 3) { return; }
			component->Translate(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
		}
		if (propertyName == "Rotate") {
			if ((int)argv.size() < 4) { return; }
			component->Rotate(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
		}
		if (propertyName == "Scale") {
			if ((int)argv.size() < 1) { return; }
			component->Scale(StringUtilities::ConvertStringToFloat(argv[0]));
		}
		if (propertyName == "LookAt") {
			if ((int)argv.size() < 3) { return; }
			component->LookAt(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
		}
	}
	
	if (componentName == "LevelManager") {
		LevelManager* component = gameObject->GetComponent<LevelManager>();
		if (component == nullptr) { return; }
		if (propertyName == "LoadLevelFromFile") {
			if ((int)argv.size() < 1) { return; }
			component->LoadLevelFromFile(ConvertStringToString(argv[0]));
		}
	}
	
	if (componentName == "SampleGameScript") {
		SampleGameScript* component = gameObject->GetComponent<SampleGameScript>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "AiPerception") {
		AiPerception* component = gameObject->GetComponent<AiPerception>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "AiRoutine") {
		AiRoutine* component = gameObject->GetComponent<AiRoutine>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "AiKnowledge") {
		AiKnowledge* component = gameObject->GetComponent<AiKnowledge>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "EnemyUnit") {
		EnemyUnit* component = gameObject->GetComponent<EnemyUnit>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "PlayerUnit") {
		PlayerUnit* component = gameObject->GetComponent<PlayerUnit>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "ShadyCamera") {
		ShadyCamera* component = gameObject->GetComponent<ShadyCamera>();
		if (component == nullptr) { return; }
		if (propertyName == "MoveTo") {
			if ((int)argv.size() < 3) { return; }
			component->MoveTo(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
		}
		if (propertyName == "MoveToRoom") {
			if ((int)argv.size() < 2) { return; }
			component->MoveToRoom(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]));
		}
		if (propertyName == "MoveToOverview") {
			if ((int)argv.size() < 0) { return; }
			component->MoveToOverview();
		}
	}
	
	if (componentName == "GridManager") {
		GridManager* component = gameObject->GetComponent<GridManager>();
		if (component == nullptr) { return; }
	}
	
	if (componentName == "GridZone") {
		GridZone* component = gameObject->GetComponent<GridZone>();
		if (component == nullptr) { return; }
		if (propertyName == "SetZoneBounds") {
			if ((int)argv.size() < 4) { return; }
			component->SetZoneBounds(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
		}
	}
	
	if (componentName == "GridNavigator") {
		GridNavigator* component = gameObject->GetComponent<GridNavigator>();
		if (component == nullptr) { return; }
		if (propertyName == "SetMovementSpeed") {
			if ((int)argv.size() < 1) { return; }
			component->SetMovementSpeed(StringUtilities::ConvertStringToFloat(argv[0]));
		}
		if (propertyName == "SetGridPosition") {
			if ((int)argv.size() < 2) { return; }
			component->SetGridPosition(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
		}
		if (propertyName == "SetDestination") {
			if ((int)argv.size() < 2) { return; }
			component->SetDestination(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
		}
		if (propertyName == "AddDestination") {
			if ((int)argv.size() < 2) { return; }
			component->AddDestination(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
		}
	}
}