#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Loaders/FontLoader/Declarations.h"
#include "Vajra/Framework/Loaders/FontLoader/FontLoader.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/Utilities.h"


namespace FontLoader {

void LoadFontSpecificationsFromCSVFile(const char* filePath,
									   float& out_fontsheetWidth, float& out_fontsheetHeight,
									   float& out_fontboxWidth, float& out_fontboxHeight,
									   int&   out_startingCharacter,
									   std::vector<float>& out_characterWidths) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading font specifications from CSV file at %s", filePath);

	std::ifstream file(filePath);

	VERIFY(file.good(), "Successfully opened CSV file at %s", filePath);

	AdvanceFileSeekTillChar (file, ',');
	out_fontsheetWidth = GetNextIntFromFile (file);
	AdvanceFileSeekTillChar (file, ',');
	out_fontsheetHeight = GetNextIntFromFile (file);

	AdvanceFileSeekTillChar (file, ',');
	out_fontboxWidth = GetNextIntFromFile (file);
	AdvanceFileSeekTillChar (file, ',');
	out_fontboxHeight = GetNextIntFromFile (file);

	AdvanceFileSeekTillChar (file, ',');
	out_startingCharacter = GetNextIntFromFile (file);

	std::string purge;
	getline (file, purge);	// purge rest of line
	getline (file, purge);	// purge the "Font Name"
	getline (file, purge);	// purge the "Font Height"
	getline (file, purge);	// purge the "Font Width"

	for (int i = 0; i < (NUM_FONT_CHARACTER_ROWS * NUM_FONT_CHARACTER_COLUMNS); i++) {
		AdvanceFileSeekTillChar (file, ',');
		out_characterWidths[i] = GetNextIntFromFile (file);								// In absolute pixels
		out_characterWidths[i] /= (out_fontsheetWidth / NUM_FONT_CHARACTER_COLUMNS);	// Scale all widths to 1.0f
	}

	file.close ();

	return;
}

}

