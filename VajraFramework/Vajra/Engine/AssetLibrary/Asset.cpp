#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"

AssetType Asset::assetType = ASSET_TYPE_GENERIC;

Asset::Asset() {
}

Asset::Asset(std::string urlOfAsset) {
	this->SetUrl(urlOfAsset);
}

Asset::~Asset() {
	ENGINE->GetAssetLibrary()->ReleaseAsset(this->GetUrl());
}

