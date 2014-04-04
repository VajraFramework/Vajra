#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Test/TestFile.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "ExampleGame/ComponentMapper/ComponentMapper.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/TagManager/TagManager.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/SavedDataManager/SavedDataManager.h"

// Forward Declarations:
void initUiGameObjects();

int TestFuntion() {
	// Intialize the game constants:
	InitGameConstants();

	// Intialize the game singletons:
	SINGLETONS; //!!!

	// Instantiate a ComponentMapper so that its singleton get stored:
	/* ComponentMapper* componentMapper = */ new ComponentMapper();




	FRAMEWORK->GetLogger()->dbglog("\nIn TestFunction()");



	// Temporary: Example bundle:
	{
		Bundle* bundle = nullptr;
		if (FRAMEWORK->GetSavedDataManager()->HasBundle("testbundle")) {
			bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle("testbundle");
		} else {
			bundle = FRAMEWORK->GetSavedDataManager()->CreateNewBundle("testbundle");
		}

		if (bundle->HasKey("pi")) { FRAMEWORK->GetLogger()->dbglog("\nPI = %f", bundle->GetFloat("pi")); }
		if (bundle->HasKey("answer")) { FRAMEWORK->GetLogger()->dbglog("\nAnswer = %i", bundle->GetInt("answer")); }
		if (bundle->HasKey("finished")) { FRAMEWORK->GetLogger()->dbglog("\nFinished = %s", bundle->GetBool("finished") ? "true" : "false"); }
		if (bundle->HasKey("gamename")) { FRAMEWORK->GetLogger()->dbglog("\nGame name = %s", bundle->GetString("gamename").c_str()); }
		if (bundle->HasKey("randomnumber")) { FRAMEWORK->GetLogger()->dbglog("\nRandom number = %s", bundle->GetString("randomnumber").c_str()); }

		int randomNumber = rand() % 1000;
		bundle->PutInt("randomnumber", randomNumber);

		bundle->Save();
	}

	return 4;
}

