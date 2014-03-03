#include "ExampleGame/Components/Switches/DecalGenerator.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/Utilities.h"

GameObject* DecalGenerator::GetDecalFromDecalType(std::string decalType, SceneGraph* scenegraph) {

	// Create a game object to display the decal:
	GameObject* decal = new GameObject(scenegraph);
	//
	SpriteRenderer* spriteRenderer = decal->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + decalType);
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
	decal->GetTransform()->Translate( 1.0f, YAXIS);
	decal->GetTransform()->Translate(-1.0f, decal->GetTransform()->GetForward());
	decal->GetTransform()->Rotate(45.0f inRadians, XAXIS);
	decal->GetTransform()->Scale(0.6f);

	return decal;
}
