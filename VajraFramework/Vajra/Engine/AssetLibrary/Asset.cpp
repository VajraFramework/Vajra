#include "Vajra/Engine/AssetLibrary/Asset.h"

AssetType Asset::assetType = ASSET_TYPE_GENERIC;

Asset::Asset() {
}

Asset::Asset(std::string urlOfAsset) {
	this->SetUrl(urlOfAsset);
}

Asset::~Asset() {
}

