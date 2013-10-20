#ifndef ASSET_H
#define ASSET_H

#include "Vajra/Engine/AssetLibrary/Declarations.h"

#include <string>

class Asset {
public:
	Asset();
	Asset(std::string urlOfAsset);
	virtual ~Asset();

	virtual AssetType GetAssetType() = 0;
	virtual void LoadAsset() = 0;

	inline std::string GetUrl() const                       { return this->url; }
	inline void        SetUrl(const std::string urlOfAsset) { this->url = urlOfAsset; }

private:
	std::string url;

	static AssetType assetType;
	friend class AssetLibrary;
};

#endif // ASSET_H
