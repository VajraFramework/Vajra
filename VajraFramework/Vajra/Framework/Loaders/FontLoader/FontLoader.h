#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <vector>
#include <string>

namespace FontLoader {

void LoadFontSpecificationsFromCSVFile(const char* filePath,
									   float& out_fontsheetWidth, float& out_fontsheetHeight,
									   float& out_fontboxWidth, float& out_fontboxHeight,
									   int&   out_startingCharacter,
									   std::vector<float>& out_characterWidths);

}

#endif // FONT_LOADER_H
