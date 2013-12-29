#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiFont/UiFontType.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <fstream>

namespace UiSceneLoader {

static void loadOneUiElement(std::ifstream& sceneFile) {
	std::string itemClass;
	std::string itemName;
	int posXPixels, posYPixels;
	int widthPixels, heightPixels;
	int zorder;
	std::string textToDisplay;
	std::string imageName;
	std::string onClickHandlerId;
	{
		// TODO [Implement]
		sceneFile >> itemClass;
		FRAMEWORK->GetLogger()->dbglog("Item class: %s", itemClass.c_str());
	}
	{
		sceneFile >> itemName;
		FRAMEWORK->GetLogger()->dbglog("Item name: %s", itemName.c_str());
	}
	{
		sceneFile >> posXPixels  >> posYPixels;
		sceneFile >> widthPixels >> heightPixels;
		// TODO [Implement]
		sceneFile >> zorder;
	}
	{
		std::string purge;
		getline (sceneFile, purge);	// purge rest of line
		// TODO [Implement]
		getline (sceneFile, textToDisplay);
		if (textToDisplay == "NULL") {
			textToDisplay = "";
		}
	}
	{
		sceneFile >> imageName;
		if (imageName == "NULL") {
			imageName = "";
		}
	}
	{
		// TODO [Implement]
		sceneFile >> onClickHandlerId;
		if (onClickHandlerId == "NULL") {
			onClickHandlerId = "";
		}
	}

	{
		// Create a new UiElement with the above properties:
		UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
		ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
		if (imageName != "") {
			uiElement->InitSprite(widthPixels, heightPixels, "sptshdr", FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesPath() + imageName);
		}
		uiElement->SetPosition(posXPixels, posYPixels);
	}
}

void LoadUiSceneFromUiSceneFile(const char* filePath) {
	FRAMEWORK->GetLogger()->dbglog("\nLoading ui scene from uiscene file %s", filePath);

	std::ifstream sceneFile(filePath);
	VERIFY(sceneFile.is_open(), "Successfully opened ui scene file at %s", filePath);

	{
		int uisceneFormatVersionNumber = -1;
		sceneFile >> uisceneFormatVersionNumber;
		VERIFY(uisceneFormatVersionNumber == UISCENE_FORMAT_VERSION_NUMBER, "Model format version number (%d) matches expected (%d)", uisceneFormatVersionNumber, UISCENE_FORMAT_VERSION_NUMBER);
	}

	int INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS;

	{
		sceneFile >> INTENDED_SCENE_WIDTH_PIXELS >> INTENDED_SCENE_HEIGHT_PIXELS;
		FRAMEWORK->GetLogger()->dbglog("Intended ui scene width, height: %d, %d", INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}

	int NUMBER_OF_UIITEMS;
	{
		sceneFile >> NUMBER_OF_UIITEMS;
		FRAMEWORK->GetLogger()->dbglog("Number of ui elements in ui scene: %d", NUMBER_OF_UIITEMS);

		for (int itemNumber = 0; itemNumber < NUMBER_OF_UIITEMS; ++itemNumber) {

			loadOneUiElement(sceneFile);

		}
	}
}

}
