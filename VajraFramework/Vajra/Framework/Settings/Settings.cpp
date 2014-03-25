#include "Vajra/Framework/Settings/Settings.h"
#include "Vajra/Framework/Settings/SettingsHelper.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

Settings::Settings() {
	// Do not call init() here
}

Settings::~Settings() {
	this->destroy();
}

SettingLevel_t Settings::GetSetting(SettingType_t settingType) {
	auto it = this->settings.find(settingType);
	ASSERT(it != this->settings.end(), "Found setting for setting type %d", settingType);
	if (it != this->settings.end()) {
		return it->second;
	}
	return SETTING_LEVEL_medium;
}

void Settings::SetSetting(SettingType_t settingType, SettingLevel_t settingLevel) {
	this->settings[settingType] = settingLevel;

	this->flushSettingsToDisk();
}

void Settings::initializeSettings() {
	if (this->readSettingsFromDisk()) {

	} else {
		// Come up with some default values:
		this->settings[SETTING_TYPE_ambient_lighting]   = SETTING_LEVEL_high;
		this->settings[SETTING_TYPE_brightness]         = SETTING_LEVEL_medium;
		this->settings[SETTING_TYPE_shadows]            = SETTING_LEVEL_medium;
		this->settings[SETTING_TYPE_texture_sampling]   = SETTING_LEVEL_high;
	}
}

bool Settings::readSettingsFromDisk() {
	if (!DoesSettingsFileExist()) {
		return false;
	}
	
	std::ifstream file;
	GetSettingsFileHandleForReading(file);

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(file);

	XmlTree* tree = parser->GetXmlTree();
	XmlNode* settingsNode = tree->GetRootNode();
	VERIFY(settingsNode->GetName() == SETTINGS_TAG_NAME, "Found settings node");

	XmlNode* settingNode = settingsNode->GetFirstChildByNodeName(SETTING_TAG_NAME);
	while (settingNode != nullptr) {
		std::string settingName  = settingNode->GetAttributeValueS(NAME_ATTRIBUTE);
		std::string settingValue = settingNode->GetAttributeValueS(VALUE_ATTRIBUTE);
		this->settings[getSettingTypeForSettingTypeName(settingName)] = getSettingLevelForSettingLevelName(settingValue);
		//
		settingNode = settingNode->GetNextSiblingByNodeName(SETTING_TAG_NAME);
	}

	delete parser;

	file.close();

	return true;
}

void Settings::flushSettingsToDisk() {
	std::ofstream file;
	GetSettingsFileHandleForWriting(file);

	file << "<settings>" << std::endl;

	for (auto it : this->settings) {
		file << "\t<setting name=\"" << getSettingTypeNameForSettingType(it.first) << "\" value=\"" << getSettingLevelNameForSettingLevel(it.second) << "\">";
		file << "</setting>" << std::endl;
	}
	file << "</settings>" << std::endl;

	file.close();
}

void Settings::init() {
	this->initializeSettings();
}

void Settings::destroy() {
}

////////////////////////////////////////////////////////////////////////////////
// Utility Functions:

// @ADD_NEW_SETTING_TYPE_HERE
SettingType_t Settings::getSettingTypeForSettingTypeName(std::string settingTypeName) {
	if (settingTypeName == "ambientlighting") { return SETTING_TYPE_ambient_lighting; }
	if (settingTypeName == "brightness") { return SETTING_TYPE_brightness; }
	if (settingTypeName == "shadows") { return SETTING_TYPE_shadows; }
	if (settingTypeName == "texturesampling") { return SETTING_TYPE_texture_sampling; }
	VERIFY(0, "Valid setting type name %s", settingTypeName.c_str());
	return SETTING_TYPE_invalid;
}

// @ADD_NEW_SETTING_TYPE_HERE
std::string Settings::getSettingTypeNameForSettingType(SettingType_t settingType) {
	switch (settingType) {
	case SETTING_TYPE_ambient_lighting: return "ambientlighting";
	case SETTING_TYPE_brightness: return "brightness";
	case SETTING_TYPE_shadows: return "shadows";
	case SETTING_TYPE_texture_sampling: return "texturesampling";
	default: VERIFY(0, "Valid setting type %d", settingType);
	}
	return "invalid";
}

// @ADD_NEW_SETTING_LEVEL_HERE
SettingLevel_t Settings::getSettingLevelForSettingLevelName(std::string settingLevelName) {
	if (settingLevelName == "off") { return SETTING_LEVEL_off; }
	if (settingLevelName == "low") { return SETTING_LEVEL_low; }
	if (settingLevelName == "medium") { return SETTING_LEVEL_medium; }
	if (settingLevelName == "high") { return SETTING_LEVEL_high; }
	VERIFY(0, "Valid setting level name %s", settingLevelName.c_str());
	return SETTING_LEVEL_invalid;
}

// @ADD_NEW_SETTING_LEVEL_HERE
std::string Settings::getSettingLevelNameForSettingLevel(SettingLevel_t settingLevel) {
	switch (settingLevel) {
	case SETTING_LEVEL_off: return "off";
	case SETTING_LEVEL_low: return "low";
	case SETTING_LEVEL_medium: return "medium";
	case SETTING_LEVEL_high: return "high";
	default: VERIFY(0, "Valid setting level %d", settingLevel);
	}
	return "invalid";
}
