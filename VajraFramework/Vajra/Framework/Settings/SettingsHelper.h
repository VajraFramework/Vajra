#ifndef SETTINGS_HELPER_H
#define SETTINGS_HELPER_H

#include <fstream>

#define SETTINGS_FILE_NAME "settings.xml"

void GetSettingsFileHandleForReading(std::ifstream& file);
void GetSettingsFileHandleForWriting(std::ofstream& file);

#define SETTINGS_TAG_NAME "settings"
#define SETTING_TAG_NAME "setting"
#define NAME_ATTRIBUTE "name"
#define VALUE_ATTRIBUTE "value"

#endif // SETTINGS_HELPER_H
