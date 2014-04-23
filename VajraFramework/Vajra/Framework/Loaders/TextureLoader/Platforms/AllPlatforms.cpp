#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Loaders/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>
#include <fstream>
#include <vector>

#define LIST_OF_TEXTURES_USING_MIPMAPS_filename "textures_using_mipmaps"

static std::vector<std::string> g_listOfTexturesUsingMipmaps;
static bool g_inited = false;

void initListOfTexturesUsingMipmaps() {
	std::ifstream listFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceMiscResourcesPath() + LIST_OF_TEXTURES_USING_MIPMAPS_filename);
	VERIFY(listFile.good(), "Successfully opened file with list of textures using mipmaps");

	while (listFile.good()) {
		std::string textureName;
		listFile >> textureName;
		//
		g_listOfTexturesUsingMipmaps.push_back(textureName);
	}

	listFile.close();

	g_inited = true;
}

bool textureMustUseMipmaps(std::string imagePath) {
	if (!g_inited) {
		initListOfTexturesUsingMipmaps();
	}

	std::size_t lastSlashPosition = imagePath.find_last_of("/");
	std::string imageName = imagePath.substr(lastSlashPosition + 1);

	if (std::find(g_listOfTexturesUsingMipmaps.begin(), g_listOfTexturesUsingMipmaps.end(), imageName) == g_listOfTexturesUsingMipmaps.end()) {
		return false;
	}
	return true;
}
