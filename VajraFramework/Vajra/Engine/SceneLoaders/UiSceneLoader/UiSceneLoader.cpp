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

void AdjustPositionForResolution(int& posX_pixels_out, int& posY_pixels_out, std::string x_wrto, std::string y_wrto, int& width_pixels_out, int& height_pixels_out, int intended_scene_width_pixels, int intended_scene_height_pixels) {
	convertPixelsFromTargetSizeToDeviceSize(posX_pixels_out,   intended_scene_width_pixels, intended_scene_height_pixels);
	convertPixelsFromTargetSizeToDeviceSize(posY_pixels_out,   intended_scene_width_pixels, intended_scene_height_pixels);
	convertPixelsFromTargetSizeToDeviceSize(width_pixels_out,  intended_scene_width_pixels, intended_scene_height_pixels);
	convertPixelsFromTargetSizeToDeviceSize(height_pixels_out, intended_scene_width_pixels, intended_scene_height_pixels);

	// Process relative positioning tags, if any:
	if (x_wrto == "LEFT") {
		// Nothing to do
	} else if (x_wrto == "RIGHT") {
		posX_pixels_out = FRAMEWORK->GetDeviceProperties()->GetWidthPixels() - posX_pixels_out - width_pixels_out;
	} else if (x_wrto == "CENTER") {
		posX_pixels_out = FRAMEWORK->GetDeviceProperties()->GetWidthPixels() / 2.0f + posX_pixels_out;
	} else {
		ASSERT(0, "Valid x wrto field: %s", x_wrto.c_str());
	}

	if (y_wrto == "TOP") {
		// Nothing to do
	} else if (y_wrto == "BOTTOM") {
		posY_pixels_out = FRAMEWORK->GetDeviceProperties()->GetHeightPixels() - posY_pixels_out - height_pixels_out;
	} else if (y_wrto == "CENTER") {
		posY_pixels_out = FRAMEWORK->GetDeviceProperties()->GetHeightPixels() / 2.0f + posY_pixels_out;
	} else {
		ASSERT(0, "Valid y wrto field: %s", y_wrto.c_str());
	}
}

static void processDimensionsNode(XmlNode* dimensionsNode, int& posXPixels_out, int& posYPixels_out, int& widthPixels_out, int& heightPixels_out, int& zorder_out) {
	// Process position:
	XmlNode* positionNode = dimensionsNode->GetFirstChildByNodeName(POSITION_TAG);
	ASSERT(positionNode != nullptr, "Got valid xmlNode from dimensions node for position");
	//
	posXPixels_out = positionNode->GetAttributeValueF(X_ATTRIBUTE);
	posYPixels_out = positionNode->GetAttributeValueF(Y_ATTRIBUTE);

	// Process size:
	XmlNode* sizeNode = dimensionsNode->GetFirstChildByNodeName(SIZE_TAG);
	ASSERT(sizeNode != nullptr, "Got valid xmlNode from dimensions node for size");
	//
	widthPixels_out  = sizeNode->GetAttributeValueF(WIDTHPIXELS_ATTRIBUTE );
	heightPixels_out = sizeNode->GetAttributeValueF(HEIGHTPIXELS_ATTRIBUTE);

	// Process zorder:
	XmlNode* zorderNode = dimensionsNode->GetFirstChildByNodeName(ZORDER_TAG);
	ASSERT(zorderNode != nullptr, "Got valid xmlNode from dimensions node for zorder");
	//
	zorder_out = StringUtilities::ConvertStringToInt(zorderNode->GetValue());

	// Process relative positioning tags, if any:
	std::string x_wrto = positionNode->HasAttribute(X_WRTO_ATTRIBUTE) ? positionNode->GetAttributeValueS(X_WRTO_ATTRIBUTE) : "LEFT";
	std::string y_wrto = positionNode->HasAttribute(Y_WRTO_ATTRIBUTE) ? positionNode->GetAttributeValueS(Y_WRTO_ATTRIBUTE) : "TOP";


	// Handle FILLPARENT as a special case
	bool isFillParentX = false;
	bool isFillParentY = false;
	if (sizeNode->GetAttributeValueS(WIDTHPIXELS_ATTRIBUTE) == "FILLPARENT") {
		widthPixels_out = FRAMEWORK->GetDeviceProperties()->GetWidthPixels();
		x_wrto = "LEFT";
		posXPixels_out = 0.0f;
		isFillParentX = true;
	}
	if (sizeNode->GetAttributeValueS(HEIGHTPIXELS_ATTRIBUTE) == "FILLPARENT") {
		heightPixels_out = FRAMEWORK->GetDeviceProperties()->GetHeightPixels();
		y_wrto = "TOP";
		posYPixels_out = 0.0f;
		isFillParentY = true;
	}

	int dummy = 10;
	if (!isFillParentX) {
		AdjustPositionForResolution(posXPixels_out, dummy, x_wrto, y_wrto, widthPixels_out, dummy, INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}
	if (!isFillParentY) {
		AdjustPositionForResolution(dummy, posYPixels_out, x_wrto, y_wrto, dummy, heightPixels_out, INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);
	}
}

static void loadOneUiElement(UiElement* uiElement, XmlNode* uielementNode, UiTouchHandlers* touchHandlers) {
	std::string itemName;
	int posXPixels, posYPixels;
	int widthPixels, heightPixels;
	int zorder;
	std::string fontName;
	float fontSize;
	UiFontAlignment_type fontAlignment = UI_FONT_ALIGNMENT_left;
	glm::vec4 fontColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	std::string textToDisplay;
	std::vector<std::string> imageNames;
	glm::vec4 color;
	bool clickable;
	bool visible;
	bool imageHasTransperancy = false;

	{
		itemName = uielementNode->GetAttributeValueS(NAME_ATTRIBUTE);
		touchHandlers->uiSceneObjects[itemName] = uiElement->GetId();
		FRAMEWORK->GetLogger()->dbglog("Item name: %s", itemName.c_str());
	}
	{
		XmlNode* dimensionsNode = uielementNode->GetFirstChildByNodeName(DIMENSIONS_TAG);
		ASSERT(dimensionsNode != nullptr, "Got valid xmlNode from uielement node for dimensions");
		//
		processDimensionsNode(dimensionsNode, posXPixels, posYPixels, widthPixels, heightPixels, zorder);
		//

	}
	{
		XmlNode* textNode = uielementNode->GetFirstChildByNodeName(TEXT_TAG);
		if (textNode != nullptr) {
			XmlNode* fontNode = textNode->GetFirstChildByNodeName(FONT_TAG);
			ASSERT(fontNode != nullptr, "Got valid xmlNode from text node for font");
			fontName = fontNode->GetAttributeValueS(TYPE_ATTRIBUTE);
			fontSize = fontNode->GetAttributeValueF(SIZE_TAG);
			if (fontNode->HasAttribute(ALIGNMENT_ATTRIBUTE)) {
				std::string fontAlignment_string = fontNode->GetAttributeValueS(ALIGNMENT_ATTRIBUTE);
				if (fontAlignment_string == "LEFT")   { fontAlignment = UI_FONT_ALIGNMENT_left;   }
				if (fontAlignment_string == "CENTER") { fontAlignment = UI_FONT_ALIGNMENT_center; }
				if (fontAlignment_string == "RIGHT")  { fontAlignment = UI_FONT_ALIGNMENT_right;  }
			}
			XmlNode* fontColorNode = fontNode->GetFirstChildByNodeName(COLOR_TAG);
			if (fontColorNode != nullptr) {
				fontColor.r = fontColorNode->GetAttributeValueF(R_ATTRIBUTE);
				fontColor.g = fontColorNode->GetAttributeValueF(G_ATTRIBUTE);
				fontColor.b = fontColorNode->GetAttributeValueF(B_ATTRIBUTE);
				fontColor.a = fontColorNode->GetAttributeValueF(A_ATTRIBUTE);
			}
			convertPixelsFromTargetSizeToDeviceSize(fontSize,   INTENDED_SCENE_WIDTH_PIXELS, INTENDED_SCENE_HEIGHT_PIXELS);

			textToDisplay = textNode->GetValue();
			StringUtilities::EraseStringFromString(textToDisplay, "\n");
			StringUtilities::EraseStringFromString(textToDisplay, "\t");
		}
	}
	{
		XmlNode* imageNode = uielementNode->GetFirstChildByNodeName(IMAGE_TAG);
		while (imageNode != nullptr) {
			std::string imageName = imageNode->GetAttributeValueS(NAME_ATTRIBUTE);
			imageNames.push_back(imageName);

			if (imageNode->HasAttribute(HASTRANSPERANCY_ATTRIBUTE)) {
				imageHasTransperancy |= imageNode->GetAttributeValueB(HASTRANSPERANCY_ATTRIBUTE);
			}

			imageNode = imageNode->GetNextSiblingByNodeName(IMAGE_TAG);
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
		uiElement->SetName(itemName);
		//
		if (imageNames.size() != 0) {
			std::vector<std::string> imagePaths;
			for (std::string imageName : imageNames) {
				imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
			}
			uiElement->InitSprite(widthPixels, heightPixels, "ustshdr", imagePaths, imageHasTransperancy);
		} else {
			uiElement->InitSprite(widthPixels, heightPixels, "uscshdr", color);
		}
		//
		if (textToDisplay != "") {
			textToDisplay = StringUtilities::EraseStringFromString(textToDisplay, "\n", false);
			textToDisplay = StringUtilities::EraseStringFromString(textToDisplay, "\t", false);
			uiElement->InitTextToDisplay(textToDisplay.c_str(), widthPixels, heightPixels,
										 FRAMEWORK->GetFileSystemUtils()->GetDeviceFontResourcesFolderName() + fontName, fontSize, fontAlignment);
			uiElement->SetFontColor(fontColor);
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
		uiElement->SetZOrder(zorder);
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

	{
		SceneGraphUi* sceneGraphUi = ENGINE->GetSceneGraphUi();
		GameObject* rootGameObject = sceneGraphUi->GetRootGameObject();
		const std::list<ObjectIdType> children = rootGameObject->GetChildren();
		VERIFY(children.size() == 1, "The current ui scene should have nothing in it except for the UiTouchHandlers's eventForwarder. Did you forget to call UnloadCurrentUiScene() before calling LoadUiScene...()?");
	}
	
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

	delete parser;
}

void UnloadCurrentUiScene() {
	// remove the previous ui scene
	ENGINE->GetSceneGraphUi()->UnloadCurrentScene();
}

}

