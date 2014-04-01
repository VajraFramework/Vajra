#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

AssetType Asset::assetType = ASSET_TYPE_GENERIC;

Asset::Asset() {
}

Asset::Asset(std::string urlOfAsset) {
	this->SetUrl(urlOfAsset);
}

Asset::~Asset() {
	FRAMEWORK->GetLogger()->dbglog("\nReleasing asset at url: %s", this->GetUrl().c_str());
}

