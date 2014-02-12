#include "ExampleGame/GameConstants/GameConstants.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/StringUtilities.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#define TAG_GAME_CONSTANTS "gameconstants"
#define TAG_GAME_CONSTANT  "gameconstant"
#define TAG_NAME           "name"
#define TAG_VALUE          "value"
#define TAG_TYPE           "type"

void readGameConstantNode(XmlNode* gameconstantNode) {
	std::string name_s  = gameconstantNode->GetAttributeValueS(TAG_NAME);
	std::string type_s  = gameconstantNode->GetAttributeValueS(TAG_TYPE);
	std::string value_s = gameconstantNode->GetAttributeValueS(TAG_VALUE);

	FRAMEWORK->GetLogger()->dbglog("\nReading in game constant %s = %s", name_s.c_str(), value_s.c_str());

	VERIFY(gameConstantNameToId.find(name_s) != gameConstantNameToId.end(), "Found a game constant of name %s", name_s.c_str());
	GameConstantId_type id = gameConstantNameToId[name_s];

	if (type_s == "float") {
		gameConstants_float[id] = StringUtilities::ConvertStringToFloat(value_s);
	} else if (type_s == "string") {
		gameConstants_string[id] = value_s;
	} else if (type_s == "int") {
		gameConstants_int[id] = StringUtilities::ConvertStringToInt(value_s);
	} else if (type_s == "bool") {
		gameConstants_bool[id] = StringUtilities::ConvertStringToBool(value_s);
	} else {
		ASSERT(0, "Unknown game constant type %s", type_s.c_str());
	}
}

void ReadInGameConstants() {
	std::string filePath = FRAMEWORK->GetFileSystemUtils()->GetDeviceConstantsResourcesPath() + "gameconstants.values";
	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(filePath);

	XmlTree* tree = parser->GetXmlTree();
	XmlNode* rootNode = tree->GetRootNode();

	VERIFY(rootNode->GetName() == TAG_GAME_CONSTANTS, "Found root node of name %s", TAG_GAME_CONSTANTS);

	XmlNode* gameconstantNode = rootNode->GetFirstChildByNodeName(TAG_GAME_CONSTANT);
	while (gameconstantNode != nullptr) {
		readGameConstantNode(gameconstantNode);
		gameconstantNode = gameconstantNode->GetNextSiblingByNodeName(TAG_GAME_CONSTANT);
	}

	delete parser;

	return;
}
