#include "Vajra/Framework/Settings/Settings.h"

Settings::~Settings() {
}

Settings::Settings() {
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
	return false;
}

void Settings::flushSettingsToDisk() {
}

void Settings::init() {
	this->initializeSettings();
}

void Settings::destroy() {
}

