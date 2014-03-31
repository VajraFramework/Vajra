#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"

AssetLibrary::AssetLibrary() {
}

AssetLibrary::~AssetLibrary() {
	this->destroy();
}

void AssetLibrary::ReleaseAsset(std::string url) {
	FRAMEWORK->GetLogger()->dbglog("\nReleasing asset: %s", url.c_str());

	ASSERT(this->assetUseCount.find(url) != this->assetUseCount.end(), "Found use count for asset at url: %s", url.c_str());
	this->assetUseCount[url]--;
	if (this->assetUseCount[url] <= 0) {
		this->allAssets.erase(url);
	}
}

void AssetLibrary::init() {
}

void AssetLibrary::destroy() {
	// Need to clear all assets one by one instead of allAssets.clear() since each asset's destructor calls back to the Asset Library to remove itself
	while (!this->allAssets.empty()) {
		this->allAssets.erase(this->allAssets.begin());
	}
	this->assetUseCount.clear();
}
