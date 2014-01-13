#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <fstream>

namespace UiSceneLoader {

static int INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS;

static void convertPixelsFromTargetSizeToDeviceSize(int& out_pixels, const int targetWidthPixels, const int targetHeightPixels) {
	unsigned int actualWidthPixels  = FRAMEWORK->GetDeviceProperties()->GetWidthPixels();
	unsigned int actualHeightPixels = FRAMEWORK->GetDeviceProperties()->GetHeightPixels();
	float actualAspecRatio = (float)actualWidthPixels / (float)actualHeightPixels;
	float targetAspecRatio = (float)targetWidthPixels / (float)targetHeightPixels;

	if (actualAspecRatio > targetAspecRatio) { // Wider than expected:
		out_pixels = out_pixels * (float)actualHeightPixels / (float)targetHeightPixels;

	} else {                                   // Taller than expected:
		out_pixels = out_pixels * (float)actualWidthPixels / (float)targetWidthPixels;

	}
}

static void loadOneUiElement(std::ifstream& sceneFile, UiTouchHandlers* touchHandlers) {
	std::string itemClass;
	std::string itemName;
	int posXPixels, posYPixels;
	int widthPixels, heightPixels;
	int zorder;
	std::string fontName;
	std::string textToDisplay;
	std::string imageName;
	glm::vec4 color;
	std::string clickable;
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

		convertPixelsFromTargetSizeToDeviceSize(posXPixels,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(posYPixels,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(widthPixels,  INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(heightPixels, INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}
	{
		sceneFile >> fontName;
		if (fontName == "NULL") {
			fontName = "";
		}
	}
	{
		std::string purge;
		getline (sceneFile, purge);	// purge rest of line
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
		sceneFile >> color.r >> color.g >> color.b >> color.a;
	}
	{
		// TODO [Implement]
		sceneFile >> clickable;
		if (clickable != "true" && clickable != "false") {
			ASSERT(0, "Unknown value for clickable: %s", clickable.c_str());
		}
	}

	{
		// Create a new UiElement with the above properties:
		UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
		ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
		//
		uiElement->SetUiObjectName(itemName);
		//
		if (imageName != "") {
			uiElement->InitSprite(widthPixels, heightPixels, "sptshdr", FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesPath() + imageName);
		} else {
			uiElement->InitSprite(widthPixels, heightPixels, "spcshdr", color);
		}
		//
		if (textToDisplay != "") {
			uiElement->InitTextToDisplay(textToDisplay.c_str(), widthPixels, heightPixels,
										 FRAMEWORK->GetFileSystemUtils()->GetDeviceFontResourcesPath() + fontName);
		}
		//
		if (clickable == "true") {
			uiElement->SetClickable(true, touchHandlers);
		} else {
			uiElement->SetClickable(false);
		}
		//
		uiElement->SetPosition(posXPixels, posYPixels);
	}
}

void LoadUiSceneFromUiSceneFile(const char* filePath, UiTouchHandlers* touchHandlers) {
	FRAMEWORK->GetLogger()->dbglog("\nLoading ui scene from uiscene file %s", filePath);

	std::ifstream sceneFile(filePath);
	VERIFY(sceneFile.is_open(), "Successfully opened ui scene file at %s", filePath);

	{
		int uisceneFormatVersionNumber = -1;
		sceneFile >> uisceneFormatVersionNumber;
		VERIFY(uisceneFormatVersionNumber == UISCENE_FORMAT_VERSION_NUMBER, "Model format version number (%d) matches expected (%d)", uisceneFormatVersionNumber, UISCENE_FORMAT_VERSION_NUMBER);
	}

	{
		sceneFile >> INTENDED_SCENE_WIDTH_PIXELS >> INTENDED_SCENE_HEIGHT_PIXELS;
		FRAMEWORK->GetLogger()->dbglog("Intended ui scene width, height: %d, %d", INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}

	int NUMBER_OF_UIITEMS;
	{
		sceneFile >> NUMBER_OF_UIITEMS;
		FRAMEWORK->GetLogger()->dbglog("Number of ui elements in ui scene: %d", NUMBER_OF_UIITEMS);

		for (int itemNumber = 0; itemNumber < NUMBER_OF_UIITEMS; ++itemNumber) {

			loadOneUiElement(sceneFile, touchHandlers);

		}
	}
}

}
