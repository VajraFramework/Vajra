#ifndef SETTINGS_H
#define SETTINGS_H

#include "Vajra/Common/Objects/Object.h"

#include <map>
#include <string>

enum SettingType_t {
	SETTING_TYPE_texture_sampling,
	SETTING_TYPE_shadows,
	SETTING_TYPE_ambient_lighting,
	SETTING_TYPE_brightness,
	//
	SETTING_TYPE_num_settings,
};

enum SettingLevel_t {
	SETTING_LEVEL_off,
	SETTING_LEVEL_low,
	SETTING_LEVEL_medium,
	SETTING_LEVEL_high,
	//
	SETTING_LEVEL_num_setting_levels,
};

class Settings : public Object {
public:
    ~Settings();

    SettingLevel_t GetSetting(SettingType_t settingType);
    void           SetSetting(SettingType_t settingType, SettingLevel_t settingLevel);

private:
    Settings();
    void init();
    void destroy();

    // Utility Functions:
    void initializeSettings();
    bool readSettingsFromDisk();
    void flushSettingsToDisk();

    std::map<SettingType_t, SettingLevel_t> settings;

    friend class Framework;
};

#endif // SETTINGS_H
