#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Vajra/Utilities/XmlParser/XmlParser.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"

#include <fstream>

namespace UiSceneLoader {

static int INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS;

static void convertPixelsFromTargetSizeToDeviceSize(float& out_pixels, const int targetWidthPixels, const int targetHeightPixels) {
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

static void convertPixelsFromTargetSizeToDeviceSize(int& out_pixels, const int targetWidthPixels, const int targetHeightPixels) {
	float out_pixelsf = out_pixels;
	convertPixelsFromTargetSizeToDeviceSize(out_pixelsf, targetWidthPixels, targetHeightPixels);
	out_pixels = out_pixelsf;
}

static void loadOneUiElement(UiElement* uiElement, XmlNode* uielementNode, UiTouchHandlers* touchHandlers) {
	std::string itemName;
	int posXPixels, posYPixels;
	int widthPixels, heightPixels;
	int zorder;
	std::string fontName;
	float fontSize;
	std::string textToDisplay;
	std::string imageName;
	glm::vec4 color;
	bool clickable;
	bool visible;

	{
		itemName = uielementNode->GetAttributeValueS(NAME_ATTRIBUTE);
		touchHandlers->uiSceneObjects[itemName] = uiElement->GetId();
		FRAMEWORK->GetLogger()->dbglog("Item name: %s", itemName.c_str());
	}
	{
		XmlNode* dimensionsNode = uielementNode->GetFirstChildByNodeName(DIMENSIONS_TAG);
		ASSERT(dimensionsNode != nullptr, "Got valid xmlNode from uielement node for dimensions");
		XmlNode* positionNode = dimensionsNode->GetFirstChildByNodeName(POSITION_TAG);
		ASSERT(positionNode != nullptr, "Got valid xmlNode from dimensions node for position");
		XmlNode* sizeNode = dimensionsNode->GetFirstChildByNodeName(SIZE_TAG);
		ASSERT(sizeNode != nullptr, "Got valid xmlNode from dimensions node for size");
		//
		posXPixels = positionNode->GetAttributeValueF(X_ATTRIBUTE);
		posYPixels = positionNode->GetAttributeValueF(Y_ATTRIBUTE);
		widthPixels  = sizeNode->GetAttributeValueF(WIDTHPIXELS_ATTRIBUTE );
		heightPixels = sizeNode->GetAttributeValueF(HEIGHTPIXELS_ATTRIBUTE);

		XmlNode* zorderNode = dimensionsNode->GetFirstChildByNodeName(ZORDER_TAG);
		ASSERT(zorderNode != nullptr, "Got valid xmlNode from dimensions node for zorder");
		// TODO [Implement]
		zorder = StringUtilities::ConvertStringToInt(zorderNode->GetValue());
		// TODO [Cleanup] Temp, using variable
		FRAMEWORK->GetLogger()->dbglog("\nzorder (temp, using variable): %d", zorder);

		convertPixelsFromTargetSizeToDeviceSize(posXPixels,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(posYPixels,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(widthPixels,  INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
		convertPixelsFromTargetSizeToDeviceSize(heightPixels, INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}
	{
		XmlNode* textNode = uielementNode->GetFirstChildByNodeName(TEXT_TAG);
		if (textNode != nullptr) {
			XmlNode* fontNode = textNode->GetFirstChildByNodeName(FONT_TAG);
			ASSERT(fontNode != nullptr, "Got valid xmlNode from text node for font");
			fontName = fontNode->GetAttributeValueS(TYPE_ATTRIBUTE);
			fontSize = fontNode->GetAttributeValueF(SIZE_TAG);
			convertPixelsFromTargetSizeToDeviceSize(fontSize,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);

			textToDisplay = textNode->GetValue();
			StringUtilities::EraseStringFromString(textToDisplay, "\n");
			StringUtilities::EraseStringFromString(textToDisplay, "\t");
		}
	}
	{
		XmlNode* imageNode = uielementNode->GetFirstChildByNodeName(IMAGE_TAG);
		if (imageNode != nullptr) {
			imageName = imageNode->GetAttributeValueS(NAME_ATTRIBUTE);
		}
	}
	{
		XmlNode* colorNode = uielementNode->GetFirstChildByNodeName(COLOR_TAG);
		if (colorNode != nullptr) {
			color.r = colorNode->GetAttributeValueF(R_ATTRIBUTE);
			color.g = colorNode->GetAttributeValueF(G_ATTRIBUTE);
			color.b = colorNode->GetAttributeValueF(B_ATTRIBUTE);
			color.a = colorNode->GetAttributeValueF(A_ATTRIBUTE);
		}
	}
	{
		clickable = uielementNode->GetAttributeValueB(CLICKABLE_ATTRIBUTE);
	}
	{
		if(uielementNode->HasAttribute(VISIBLE_ATTRIBUTE)) {
			visible = uielementNode->GetAttributeValueB(VISIBLE_ATTRIBUTE);
		} else {
			visible = true;
		}
	}
	{
		uiElement->SetUiObjectName(itemName);
		//
		if (imageName != "") {
			uiElement->InitSprite(widthPixels, heightPixels, "sptshdr", FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
		} else {
			uiElement->InitSprite(widthPixels, heightPixels, "spcshdr", color);
		}
		//
		if (textToDisplay != "") {
			uiElement->InitTextToDisplay(textToDisplay.c_str(), widthPixels, heightPixels,
										 FRAMEWORK->GetFileSystemUtils()->GetDeviceFontResourcesFolderName() + fontName, fontSize);
		}
		//
		if (clickable == true) {
			uiElement->SetTouchHandlers(touchHandlers);
			uiElement->SetClickable(true);
		} else {
			uiElement->SetClickable(false);
		}
		//
		// TODO [Cleanup] remove this when visible works correctly
		if(uiElement->GetParentId() != OBJECT_ID_INVALID) {
			bool parentVis = (GameObject*)uiElement->GetParentSceneGraph()->GetGameObjectById(uiElement->GetParentId())->IsVisible();
			if(!parentVis) {
				visible = false;
			}
		}
		uiElement->SetVisible(visible);
		//
		uiElement->SetPosition(posXPixels, posYPixels);
	}

	XmlNode* childNode = uielementNode->GetFirstChildByNodeName(UIELEMENT_TAG);
	while (childNode != nullptr) {
		UiElement* childUiElement = new UiElement(ENGINE->GetSceneGraphUi());
		ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(childUiElement->GetId());
		uiElement->AddChild(childUiElement->GetId());

		loadOneUiElement(childUiElement, childNode, touchHandlers);

		childNode = childNode->GetNextSiblingByNodeName(UIELEMENT_TAG);
	}
}

void LoadUiSceneFromUiSceneFile(std::string filePath, UiTouchHandlers* touchHandlers) {
	FRAMEWORK->GetLogger()->dbglog("\nLoading ui scene from uiscene file %s", filePath.c_str());

	// remove the preivous ui scene
	ENGINE->GetSceneGraphUi()->UnloadCurrentScene();

	
	//Todo [HACK] remove this when cameras don't get destroy by unload scene
	GameObject* camera = new GameObject(ENGINE->GetSceneGraphUi());
	ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(camera->GetId());
	Camera* cameraComponent = camera->AddComponent<Camera>();
	cameraComponent->SetCameraType(CAMERA_TYPE_ORTHO);
	camera->GetTransform()->SetPosition(0.0f, 0.0f, 400.0f);
	camera->GetTransform()->Rotate(10.0f, YAXIS);
	camera->GetTransform()->LookAt(0.0f, 0.0f, 0.0f);
	ENGINE->GetSceneGraphUi()->SetMainCameraId(camera->GetId());

	std::ifstream sceneFile(filePath);
	VERIFY(sceneFile.is_open(), "Successfully opened ui scene file at %s", filePath.c_str());

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(filePath);

#ifdef DEBUG
	parser->Print();
#endif // DEBUG

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for uiscene file %s", filePath.c_str());
	XmlNode* uisceneNode = xmlTree->GetRootNode();
	ASSERT(uisceneNode != nullptr && uisceneNode->GetName() == UISCENE_TAG, "Got valid uiscene node from xml tree for uiscene file %s", filePath.c_str());

	{
		INTENDED_SCENE_WIDTH_PIXELS  = uisceneNode->GetAttributeValueF(WIDTHPIXELS_ATTRIBUTE );
		INTENDED_SCENE_HEIGHT_PIXELS = uisceneNode->GetAttributeValueF(HEIGHTPIXELS_ATTRIBUTE);
		FRAMEWORK->GetLogger()->dbglog("Intended ui scene width, height: %d, %d", INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}

	{
		XmlNode* uielementNode = uisceneNode->GetFirstChildByNodeName(UIELEMENT_TAG);
		while (uielementNode != nullptr) {
			UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
			ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
			loadOneUiElement(uiElement, uielementNode, touchHandlers);

			uielementNode = uielementNode->GetNextSiblingByNodeName(UIELEMENT_TAG);
		}
	}
}

}
