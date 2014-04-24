#include "ExampleGame/ComponentMapper/ComponentMapper.h"
#include "Vajra/Utilities/StringUtilities.h"

#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/WaterRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/ParticleSystemRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioListener.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "ExampleGame/Components/Triggers/TriggerMovingBlocker.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "ExampleGame/Components/Triggers/TriggerLoot.h"
#include "ExampleGame/Components/Triggers/TriggerTransformation.h"
#include "ExampleGame/Components/Triggers/TriggerTerrainBlock.h"
#include "ExampleGame/Components/Triggers/TriggerElevationChange.h"
#include "ExampleGame/Components/Triggers/TriggerMultiplex.h"
#include "ExampleGame/Components/Triggers/TriggerParticles.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/LevelManager/MasteryManager.h"
#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitShadow.h"
#include "ExampleGame/Components/GameScripts/UnitCustomizers/UnitAnimationManager.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/GameScripts/Units/Guard.h"
#include "ExampleGame/Components/GameScripts/Units/Obstacles/BreakablePot.h"
#include "ExampleGame/Components/GameScripts/Units/Obstacles/PushPillar.h"
#include "ExampleGame/Components/Decals/Decal.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Components/Switches/UnitDeathSwitch.h"
#include "ExampleGame/Components/Switches/MultiplexSwitch.h"
#include "ExampleGame/Components/Switches/GoalZoneSwitch.h"
#include "ExampleGame/Components/Switches/UnitInGridZoneSwitch.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Switches/GridZoneSwitch.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZones/TransitionZone.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"


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
	
	if (componentName == "WaterRenderer") {
		WaterRenderer* component = gameObject->GetComponent<WaterRenderer>();
		if (component == nullptr) { component = gameObject->AddComponent<WaterRenderer>(); }
		return component;
	}
	
	if (componentName == "ParticleSystemRenderer") {
		ParticleSystemRenderer* component = gameObject->GetComponent<ParticleSystemRenderer>();
		if (component == nullptr) { component = gameObject->AddComponent<ParticleSystemRenderer>(); }
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
	
	if (componentName == "AudioListener") {
		AudioListener* component = gameObject->GetComponent<AudioListener>();
		if (component == nullptr) { component = gameObject->AddComponent<AudioListener>(); }
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
	
	if (componentName == "ParticleSystem") {
		ParticleSystem* component = gameObject->GetComponent<ParticleSystem>();
		if (component == nullptr) { component = gameObject->AddComponent<ParticleSystem>(); }
		return component;
	}
	
	if (componentName == "TriggerMovingBlocker") {
		TriggerMovingBlocker* component = gameObject->GetComponent<TriggerMovingBlocker>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerMovingBlocker>(); }
		return component;
	}
	
	if (componentName == "Triggerable") {
		Triggerable* component = gameObject->GetComponent<Triggerable>();
		if (component == nullptr) { component = gameObject->AddComponent<Triggerable>(); }
		return component;
	}
	
	if (componentName == "TriggerLoot") {
		TriggerLoot* component = gameObject->GetComponent<TriggerLoot>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerLoot>(); }
		return component;
	}
	
	if (componentName == "TriggerTransformation") {
		TriggerTransformation* component = gameObject->GetComponent<TriggerTransformation>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerTransformation>(); }
		return component;
	}
	
	if (componentName == "TriggerTerrainBlock") {
		TriggerTerrainBlock* component = gameObject->GetComponent<TriggerTerrainBlock>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerTerrainBlock>(); }
		return component;
	}
	
	if (componentName == "TriggerElevationChange") {
		TriggerElevationChange* component = gameObject->GetComponent<TriggerElevationChange>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerElevationChange>(); }
		return component;
	}
	
	if (componentName == "TriggerMultiplex") {
		TriggerMultiplex* component = gameObject->GetComponent<TriggerMultiplex>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerMultiplex>(); }
		return component;
	}
	
	if (componentName == "TriggerParticles") {
		TriggerParticles* component = gameObject->GetComponent<TriggerParticles>();
		if (component == nullptr) { component = gameObject->AddComponent<TriggerParticles>(); }
		return component;
	}
	
	if (componentName == "LevelManager") {
		LevelManager* component = gameObject->GetComponent<LevelManager>();
		if (component == nullptr) { component = gameObject->AddComponent<LevelManager>(); }
		return component;
	}
	
	if (componentName == "MasteryManager") {
		MasteryManager* component = gameObject->GetComponent<MasteryManager>();
		if (component == nullptr) { component = gameObject->AddComponent<MasteryManager>(); }
		return component;
	}
	
	if (componentName == "SampleGameScript") {
		SampleGameScript* component = gameObject->GetComponent<SampleGameScript>();
		if (component == nullptr) { component = gameObject->AddComponent<SampleGameScript>(); }
		return component;
	}
	
	if (componentName == "UnitShadow") {
		UnitShadow* component = gameObject->GetComponent<UnitShadow>();
		if (component == nullptr) { component = gameObject->AddComponent<UnitShadow>(); }
		return component;
	}
	
	if (componentName == "UnitAnimationManager") {
		UnitAnimationManager* component = gameObject->GetComponent<UnitAnimationManager>();
		if (component == nullptr) { component = gameObject->AddComponent<UnitAnimationManager>(); }
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
	
	if (componentName == "Thief") {
		Thief* component = gameObject->GetComponent<Thief>();
		if (component == nullptr) { component = gameObject->AddComponent<Thief>(); }
		return component;
	}
	
	if (componentName == "Assassin") {
		Assassin* component = gameObject->GetComponent<Assassin>();
		if (component == nullptr) { component = gameObject->AddComponent<Assassin>(); }
		return component;
	}
	
	if (componentName == "Guard") {
		Guard* component = gameObject->GetComponent<Guard>();
		if (component == nullptr) { component = gameObject->AddComponent<Guard>(); }
		return component;
	}
	
	if (componentName == "BreakablePot") {
		BreakablePot* component = gameObject->GetComponent<BreakablePot>();
		if (component == nullptr) { component = gameObject->AddComponent<BreakablePot>(); }
		return component;
	}
	
	if (componentName == "PushPillar") {
		PushPillar* component = gameObject->GetComponent<PushPillar>();
		if (component == nullptr) { component = gameObject->AddComponent<PushPillar>(); }
		return component;
	}
	
	if (componentName == "Decal") {
		Decal* component = gameObject->GetComponent<Decal>();
		if (component == nullptr) { component = gameObject->AddComponent<Decal>(); }
		return component;
	}
	
	if (componentName == "ShadyCamera") {
		ShadyCamera* component = gameObject->GetComponent<ShadyCamera>();
		if (component == nullptr) { component = gameObject->AddComponent<ShadyCamera>(); }
		return component;
	}
	
	if (componentName == "UnitDeathSwitch") {
		UnitDeathSwitch* component = gameObject->GetComponent<UnitDeathSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<UnitDeathSwitch>(); }
		return component;
	}
	
	if (componentName == "MultiplexSwitch") {
		MultiplexSwitch* component = gameObject->GetComponent<MultiplexSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<MultiplexSwitch>(); }
		return component;
	}
	
	if (componentName == "GoalZoneSwitch") {
		GoalZoneSwitch* component = gameObject->GetComponent<GoalZoneSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<GoalZoneSwitch>(); }
		return component;
	}
	
	if (componentName == "UnitInGridZoneSwitch") {
		UnitInGridZoneSwitch* component = gameObject->GetComponent<UnitInGridZoneSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<UnitInGridZoneSwitch>(); }
		return component;
	}
	
	if (componentName == "BaseSwitch") {
		BaseSwitch* component = gameObject->GetComponent<BaseSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<BaseSwitch>(); }
		return component;
	}
	
	if (componentName == "GridZoneSwitch") {
		GridZoneSwitch* component = gameObject->GetComponent<GridZoneSwitch>();
		if (component == nullptr) { component = gameObject->AddComponent<GridZoneSwitch>(); }
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
	
	if (componentName == "TransitionZone") {
		TransitionZone* component = gameObject->GetComponent<TransitionZone>();
		if (component == nullptr) { component = gameObject->AddComponent<TransitionZone>(); }
		return component;
	}
	
	if (componentName == "MenuManager") {
		MenuManager* component = gameObject->GetComponent<MenuManager>();
		if (component == nullptr) { component = gameObject->AddComponent<MenuManager>(); }
		return component;
	}

	return nullptr;
}


void ComponentMapper::InitializePropertyByComponentAndPropertyNames(GameObject *gameObject, std::string componentName, std::string propertyName, std::vector<std::string> argv) {

	
	if (componentName == "RigidAnimation") {
		RigidAnimation* component = gameObject->GetComponent<RigidAnimation>();
		if (component == nullptr) { return; }
		if (propertyName == "AddAnimationClip") {
			if ((int)argv.size() < 3) { return; }
			component->AddAnimationClip(ConvertStringToString(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToBool(argv[2]));
			return;
		}
		if (propertyName == "PlayAnimationClip") {
			if ((int)argv.size() < 1) { return; }
			component->PlayAnimationClip(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "BakedSkeletalAnimation") {
		BakedSkeletalAnimation* component = gameObject->GetComponent<BakedSkeletalAnimation>();
		if (component == nullptr) { return; }
		if (propertyName == "AddAnimationClip") {
			if ((int)argv.size() < 3) { return; }
			component->AddAnimationClip(ConvertStringToString(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToBool(argv[2]));
			return;
		}
		if (propertyName == "PlayAnimationClip") {
			if ((int)argv.size() < 1) { return; }
			component->PlayAnimationClip(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "Camera") {
		Camera* component = gameObject->GetComponent<Camera>();
		if (component == nullptr) { return; }
		if (propertyName == "SetCameraType") {
			if ((int)argv.size() < 1) { return; }
			component->SetCameraType(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetFOV") {
			if ((int)argv.size() < 1) { return; }
			component->SetFOV(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "DirectionalLight") {
		DirectionalLight* component = gameObject->GetComponent<DirectionalLight>();
		if (component == nullptr) { return; }
		if (propertyName == "SetIntensity") {
			if ((int)argv.size() < 1) { return; }
			component->SetIntensity(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetAmbientColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetAmbientColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetAmbientColorInts") {
			if ((int)argv.size() < 4) { return; }
			component->SetAmbientColorInts(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
			return;
		}
		if (propertyName == "SetDiffuseColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetDiffuseColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetDiffuseColorInts") {
			if ((int)argv.size() < 4) { return; }
			component->SetDiffuseColorInts(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
			return;
		}
		if (propertyName == "SetSpecularColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetSpecularColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetLightType") {
			if ((int)argv.size() < 1) { return; }
			component->SetLightType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "WaterRenderer") {
		WaterRenderer* component = gameObject->GetComponent<WaterRenderer>();
		if (component == nullptr) { return; }
		if (propertyName == "InitMesh") {
			if ((int)argv.size() < 1) { return; }
			component->InitMesh(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetScrollingUVs") {
			if ((int)argv.size() < 1) { return; }
			component->SetScrollingUVs(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetSecondaryTexture") {
			if ((int)argv.size() < 1) { return; }
			component->SetSecondaryTexture(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "ParticleSystemRenderer") {
		ParticleSystemRenderer* component = gameObject->GetComponent<ParticleSystemRenderer>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "MeshRenderer") {
		MeshRenderer* component = gameObject->GetComponent<MeshRenderer>();
		if (component == nullptr) { return; }
		if (propertyName == "InitMesh") {
			if ((int)argv.size() < 1) { return; }
			component->InitMesh(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetIsStatic") {
			if ((int)argv.size() < 1) { return; }
			component->SetIsStatic(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "AudioSource") {
		AudioSource* component = gameObject->GetComponent<AudioSource>();
		if (component == nullptr) { return; }
		if (propertyName == "LoadAudioClip") {
			if ((int)argv.size() < 2) { return; }
			component->LoadAudioClip(ConvertStringToString(argv[0]), ConvertStringToString(argv[1]));
			return;
		}
		if (propertyName == "SetAudioClip") {
			if ((int)argv.size() < 1) { return; }
			component->SetAudioClip(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetVolume") {
			if ((int)argv.size() < 1) { return; }
			component->SetVolume(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetPlaybackSpeed") {
			if ((int)argv.size() < 1) { return; }
			component->SetPlaybackSpeed(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetLooping") {
			if ((int)argv.size() < 1) { return; }
			component->SetLooping(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetSourceIs3D") {
			if ((int)argv.size() < 1) { return; }
			component->SetSourceIs3D(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetPlayOnlyWhenVisible") {
			if ((int)argv.size() < 1) { return; }
			component->SetPlayOnlyWhenVisible(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "AudioListener") {
		AudioListener* component = gameObject->GetComponent<AudioListener>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "Armature") {
		Armature* component = gameObject->GetComponent<Armature>();
		if (component == nullptr) { return; }
		if (propertyName == "InitArmature") {
			if ((int)argv.size() < 1) { return; }
			component->InitArmature(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "Transform") {
		Transform* component = gameObject->GetComponent<Transform>();
		if (component == nullptr) { return; }
		if (propertyName == "SetPosition") {
			if ((int)argv.size() < 3) { return; }
			component->SetPosition(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetOrientation") {
			if ((int)argv.size() < 4) { return; }
			component->SetOrientation(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetScale") {
			if ((int)argv.size() < 3) { return; }
			component->SetScale(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "Translate") {
			if ((int)argv.size() < 3) { return; }
			component->Translate(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "Rotate") {
			if ((int)argv.size() < 4) { return; }
			component->Rotate(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "RotateByDegrees") {
			if ((int)argv.size() < 4) { return; }
			component->RotateByDegrees(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "Scale") {
			if ((int)argv.size() < 1) { return; }
			component->Scale(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "LookAt") {
			if ((int)argv.size() < 3) { return; }
			component->LookAt(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		return;
	}
	
	if (componentName == "ParticleSystem") {
		ParticleSystem* component = gameObject->GetComponent<ParticleSystem>();
		if (component == nullptr) { return; }
		if (propertyName == "SetNumberOfParticles") {
			if ((int)argv.size() < 2) { return; }
			component->SetNumberOfParticles(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
			return;
		}
		if (propertyName == "SetParticleSpeed") {
			if ((int)argv.size() < 2) { return; }
			component->SetParticleSpeed(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]));
			return;
		}
		if (propertyName == "SetParticleSize") {
			if ((int)argv.size() < 2) { return; }
			component->SetParticleSize(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]));
			return;
		}
		if (propertyName == "SetParticleLifespan") {
			if ((int)argv.size() < 1) { return; }
			component->SetParticleLifespan(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetParticleTexture") {
			if ((int)argv.size() < 1) { return; }
			component->SetParticleTexture(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetParticleInitialColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetParticleInitialColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetParticleFinalColor") {
			if ((int)argv.size() < 4) { return; }
			component->SetParticleFinalColor(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetParticleVelocityDirection") {
			if ((int)argv.size() < 3) { return; }
			component->SetParticleVelocityDirection(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetParticleVelocityDirectionRandomness") {
			if ((int)argv.size() < 1) { return; }
			component->SetParticleVelocityDirectionRandomness(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetAccelerationAmount") {
			if ((int)argv.size() < 1) { return; }
			component->SetAccelerationAmount(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetAccelerationDirection") {
			if ((int)argv.size() < 3) { return; }
			component->SetAccelerationDirection(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetEmissionVolume") {
			if ((int)argv.size() < 4) { return; }
			component->SetEmissionVolume(ConvertStringToString(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetOverallLifespan") {
			if ((int)argv.size() < 1) { return; }
			component->SetOverallLifespan(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetSimulateInWorldSpace") {
			if ((int)argv.size() < 1) { return; }
			component->SetSimulateInWorldSpace(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetName") {
			if ((int)argv.size() < 1) { return; }
			component->SetName(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "InitParticleSystem") {
			if ((int)argv.size() < 0) { return; }
			component->InitParticleSystem();
			return;
		}
		if (propertyName == "Play") {
			if ((int)argv.size() < 0) { return; }
			component->Play();
			return;
		}
		if (propertyName == "Pause") {
			if ((int)argv.size() < 0) { return; }
			component->Pause();
			return;
		}
		if (propertyName == "Stop") {
			if ((int)argv.size() < 0) { return; }
			component->Stop();
			return;
		}
		if (propertyName == "SetLooping") {
			if ((int)argv.size() < 1) { return; }
			component->SetLooping(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "TriggerMovingBlocker") {
		TriggerMovingBlocker* component = gameObject->GetComponent<TriggerMovingBlocker>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetToggleState") {
			if ((int)argv.size() < 1) { return; }
			component->SetToggleState(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetChangeOnEnterCell") {
			if ((int)argv.size() < 1) { return; }
			component->SetChangeOnEnterCell(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetChangeWalkability") {
			if ((int)argv.size() < 1) { return; }
			component->SetChangeWalkability(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetChangeVisibility") {
			if ((int)argv.size() < 1) { return; }
			component->SetChangeVisibility(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetTranslation") {
			if ((int)argv.size() < 3) { return; }
			component->SetTranslation(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetTransitTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetTransitTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "Triggerable") {
		Triggerable* component = gameObject->GetComponent<Triggerable>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetToggleState") {
			if ((int)argv.size() < 1) { return; }
			component->SetToggleState(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "TriggerLoot") {
		TriggerLoot* component = gameObject->GetComponent<TriggerLoot>();
		if (component == nullptr) { return; }
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		if (propertyName == "SetMoneyValue") {
			if ((int)argv.size() < 1) { return; }
			component->SetMoneyValue(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetActive") {
			if ((int)argv.size() < 1) { return; }
			component->SetActive(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "TriggerTransformation") {
		TriggerTransformation* component = gameObject->GetComponent<TriggerTransformation>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetTranslation") {
			if ((int)argv.size() < 3) { return; }
			component->SetTranslation(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetRotationDegrees") {
			if ((int)argv.size() < 4) { return; }
			component->SetRotationDegrees(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetRotationRadians") {
			if ((int)argv.size() < 4) { return; }
			component->SetRotationRadians(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		if (propertyName == "SetScaling") {
			if ((int)argv.size() < 3) { return; }
			component->SetScaling(StringUtilities::ConvertStringToFloat(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), StringUtilities::ConvertStringToFloat(argv[2]));
			return;
		}
		if (propertyName == "SetTransitTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetTransitTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetTransformedState") {
			if ((int)argv.size() < 1) { return; }
			component->SetTransformedState(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		return;
	}
	
	if (componentName == "TriggerTerrainBlock") {
		TriggerTerrainBlock* component = gameObject->GetComponent<TriggerTerrainBlock>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetChangeWalkability") {
			if ((int)argv.size() < 1) { return; }
			component->SetChangeWalkability(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetChangeVisibility") {
			if ((int)argv.size() < 1) { return; }
			component->SetChangeVisibility(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetPassableWhenActivated") {
			if ((int)argv.size() < 1) { return; }
			component->SetPassableWhenActivated(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		return;
	}
	
	if (componentName == "TriggerElevationChange") {
		TriggerElevationChange* component = gameObject->GetComponent<TriggerElevationChange>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetElevationDiff") {
			if ((int)argv.size() < 1) { return; }
			component->SetElevationDiff(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetTransitTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetTransitTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetRaisedState") {
			if ((int)argv.size() < 1) { return; }
			component->SetRaisedState(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		return;
	}
	
	if (componentName == "TriggerMultiplex") {
		TriggerMultiplex* component = gameObject->GetComponent<TriggerMultiplex>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetToggleState") {
			if ((int)argv.size() < 1) { return; }
			component->SetToggleState(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "TriggerParticles") {
		TriggerParticles* component = gameObject->GetComponent<TriggerParticles>();
		if (component == nullptr) { return; }
		if (propertyName == "SetTriggerType") {
			if ((int)argv.size() < 1) { return; }
			component->SetTriggerType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetLooped") {
			if ((int)argv.size() < 1) { return; }
			component->SetLooped(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SubscribeToMySwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToMySwitch();
			return;
		}
		if (propertyName == "SubscribeToParentSwitch") {
			if ((int)argv.size() < 0) { return; }
			component->SubscribeToParentSwitch();
			return;
		}
		return;
	}
	
	if (componentName == "LevelManager") {
		LevelManager* component = gameObject->GetComponent<LevelManager>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "MasteryManager") {
		MasteryManager* component = gameObject->GetComponent<MasteryManager>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "SampleGameScript") {
		SampleGameScript* component = gameObject->GetComponent<SampleGameScript>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "UnitShadow") {
		UnitShadow* component = gameObject->GetComponent<UnitShadow>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "UnitAnimationManager") {
		UnitAnimationManager* component = gameObject->GetComponent<UnitAnimationManager>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "AiPerception") {
		AiPerception* component = gameObject->GetComponent<AiPerception>();
		if (component == nullptr) { return; }
		if (propertyName == "SetVisionRange") {
			if ((int)argv.size() < 1) { return; }
			component->SetVisionRange(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetFieldOfVision") {
			if ((int)argv.size() < 1) { return; }
			component->SetFieldOfVision(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetFieldOfVisionDegrees") {
			if ((int)argv.size() < 1) { return; }
			component->SetFieldOfVisionDegrees(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetVisionAcuity") {
			if ((int)argv.size() < 1) { return; }
			component->SetVisionAcuity(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "AiRoutine") {
		AiRoutine* component = gameObject->GetComponent<AiRoutine>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "AiKnowledge") {
		AiKnowledge* component = gameObject->GetComponent<AiKnowledge>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "Thief") {
		Thief* component = gameObject->GetComponent<Thief>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "Assassin") {
		Assassin* component = gameObject->GetComponent<Assassin>();
		if (component == nullptr) { return; }
		if (propertyName == "SetDashEffect") {
			if ((int)argv.size() < 1) { return; }
			component->SetDashEffect(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetSpecialHitEffect") {
			if ((int)argv.size() < 1) { return; }
			component->SetSpecialHitEffect(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "Guard") {
		Guard* component = gameObject->GetComponent<Guard>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "BreakablePot") {
		BreakablePot* component = gameObject->GetComponent<BreakablePot>();
		if (component == nullptr) { return; }
		if (propertyName == "SetDeathEffect") {
			if ((int)argv.size() < 1) { return; }
			component->SetDeathEffect(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetHasPotLoot") {
			if ((int)argv.size() < 0) { return; }
			component->SetHasPotLoot();
			return;
		}
		return;
	}
	
	if (componentName == "PushPillar") {
		PushPillar* component = gameObject->GetComponent<PushPillar>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSlideEffect") {
			if ((int)argv.size() < 1) { return; }
			component->SetSlideEffect(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "Decal") {
		Decal* component = gameObject->GetComponent<Decal>();
		if (component == nullptr) { return; }
		if (propertyName == "SetDecalImages") {
			if ((int)argv.size() < 4) { return; }
			component->SetDecalImages(ConvertStringToString(argv[0]), StringUtilities::ConvertStringToFloat(argv[1]), ConvertStringToString(argv[2]), StringUtilities::ConvertStringToFloat(argv[3]));
			return;
		}
		return;
	}
	
	if (componentName == "ShadyCamera") {
		ShadyCamera* component = gameObject->GetComponent<ShadyCamera>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "UnitDeathSwitch") {
		UnitDeathSwitch* component = gameObject->GetComponent<UnitDeathSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSwitchType") {
			if ((int)argv.size() < 1) { return; }
			component->SetSwitchType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetResetTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetResetTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetRequiredDeaths") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredDeaths(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetRequiredUnitType") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredUnitType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "AddAllowedUnitType") {
			if ((int)argv.size() < 1) { return; }
			component->AddAllowedUnitType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "AllowAllUnitTypesUpTo") {
			if ((int)argv.size() < 1) { return; }
			component->AllowAllUnitTypesUpTo(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "MultiplexSwitch") {
		MultiplexSwitch* component = gameObject->GetComponent<MultiplexSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSwitchType") {
			if ((int)argv.size() < 1) { return; }
			component->SetSwitchType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetResetTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetResetTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "GoalZoneSwitch") {
		GoalZoneSwitch* component = gameObject->GetComponent<GoalZoneSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetRequiredUnitType") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredUnitType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "UnitInGridZoneSwitch") {
		UnitInGridZoneSwitch* component = gameObject->GetComponent<UnitInGridZoneSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSwitchType") {
			if ((int)argv.size() < 1) { return; }
			component->SetSwitchType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetResetTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetResetTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetRequiredOccupants") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredOccupants(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetActivateWhenUnitsInZone") {
			if ((int)argv.size() < 1) { return; }
			component->SetActivateWhenUnitsInZone(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetRequiredUnitType") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredUnitType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "AddAllowedUnitType") {
			if ((int)argv.size() < 1) { return; }
			component->AddAllowedUnitType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "AllowAllUnitTypesUpTo") {
			if ((int)argv.size() < 1) { return; }
			component->AllowAllUnitTypesUpTo(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "BaseSwitch") {
		BaseSwitch* component = gameObject->GetComponent<BaseSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSwitchType") {
			if ((int)argv.size() < 1) { return; }
			component->SetSwitchType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetResetTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetResetTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "GridZoneSwitch") {
		GridZoneSwitch* component = gameObject->GetComponent<GridZoneSwitch>();
		if (component == nullptr) { return; }
		if (propertyName == "SetSwitchType") {
			if ((int)argv.size() < 1) { return; }
			component->SetSwitchType(ConvertStringToString(argv[0]));
			return;
		}
		if (propertyName == "SetResetTime") {
			if ((int)argv.size() < 1) { return; }
			component->SetResetTime(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetRequiredOccupants") {
			if ((int)argv.size() < 1) { return; }
			component->SetRequiredOccupants(StringUtilities::ConvertStringToInt(argv[0]));
			return;
		}
		if (propertyName == "SetActivateWhenUnitsInZone") {
			if ((int)argv.size() < 1) { return; }
			component->SetActivateWhenUnitsInZone(StringUtilities::ConvertStringToBool(argv[0]));
			return;
		}
		if (propertyName == "SetDecalType") {
			if ((int)argv.size() < 1) { return; }
			component->SetDecalType(ConvertStringToString(argv[0]));
			return;
		}
		return;
	}
	
	if (componentName == "GridManager") {
		GridManager* component = gameObject->GetComponent<GridManager>();
		if (component == nullptr) { return; }
		return;
	}
	
	if (componentName == "GridZone") {
		GridZone* component = gameObject->GetComponent<GridZone>();
		if (component == nullptr) { return; }
		if (propertyName == "SetZoneBounds") {
			if ((int)argv.size() < 4) { return; }
			component->SetZoneBounds(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
			return;
		}
		if (propertyName == "Visualize") {
			if ((int)argv.size() < 4) { return; }
			component->Visualize(StringUtilities::ConvertStringToBool(argv[0]), ConvertStringToString(argv[1]), StringUtilities::ConvertStringToBool(argv[2]), StringUtilities::ConvertStringToBool(argv[3]));
			return;
		}
		return;
	}
	
	if (componentName == "GridNavigator") {
		GridNavigator* component = gameObject->GetComponent<GridNavigator>();
		if (component == nullptr) { return; }
		if (propertyName == "SetMovementSpeed") {
			if ((int)argv.size() < 1) { return; }
			component->SetMovementSpeed(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetTurnSpeedDegrees") {
			if ((int)argv.size() < 1) { return; }
			component->SetTurnSpeedDegrees(StringUtilities::ConvertStringToFloat(argv[0]));
			return;
		}
		if (propertyName == "SetGridPosition") {
			if ((int)argv.size() < 2) { return; }
			component->SetGridPosition(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
			return;
		}
		if (propertyName == "AddDestination") {
			if ((int)argv.size() < 2) { return; }
			component->AddDestination(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
			return;
		}
		return;
	}
	
	if (componentName == "TransitionZone") {
		TransitionZone* component = gameObject->GetComponent<TransitionZone>();
		if (component == nullptr) { return; }
		if (propertyName == "SetZoneBounds") {
			if ((int)argv.size() < 4) { return; }
			component->SetZoneBounds(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
			return;
		}
		if (propertyName == "SetFirstDestination") {
			if ((int)argv.size() < 2) { return; }
			component->SetFirstDestination(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
			return;
		}
		if (propertyName == "SetSecondDestination") {
			if ((int)argv.size() < 2) { return; }
			component->SetSecondDestination(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]));
			return;
		}
		if (propertyName == "SetDestinations") {
			if ((int)argv.size() < 4) { return; }
			component->SetDestinations(StringUtilities::ConvertStringToInt(argv[0]), StringUtilities::ConvertStringToInt(argv[1]), StringUtilities::ConvertStringToInt(argv[2]), StringUtilities::ConvertStringToInt(argv[3]));
			return;
		}
		return;
	}
	
	if (componentName == "MenuManager") {
		MenuManager* component = gameObject->GetComponent<MenuManager>();
		if (component == nullptr) { return; }
		return;
	}
}