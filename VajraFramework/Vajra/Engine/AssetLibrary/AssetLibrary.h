#ifndef ASSET_LIBRARY_H
#define ASSET_LIBRARY_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <map>
#include <memory>
#include <string>

class AssetLibrary {
public:
	~AssetLibrary();

	template <class T>
	std::shared_ptr<T> GetAsset(std::string url);

private:
	AssetLibrary();
	//
	void init();
	void destroy();

	std::map<std::string /* url */, std::weak_ptr<Asset>> allAssets;

	friend class Engine;
};

////////////////////////////////////////////////////////////////////////////////

// Templated Functions:
template <class T>
std::shared_ptr<T> AssetLibrary::GetAsset(std::string url) {
	url = FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() +  "/" + url;

	std::shared_ptr<Asset> temp_sharedPtr;

	auto it = this->allAssets.find(url);
	//
	if (it != this->allAssets.end()) {
		// Found pre-loaded asset, make sure weak pointer is still valid:
		temp_sharedPtr = it->second.lock();
		if (!temp_sharedPtr) {
			// Weak pointer not valid. Asset has been recyled. Force reload:
			it = this->allAssets.end();
		}
	}
	//
	if (it == this->allAssets.end()) {
		// Asset not found in library. Create new asset and load it:
		FRAMEWORK->GetLogger()->dbglog("\nLoading asset at url: %s", url.c_str());
		temp_sharedPtr = std::static_pointer_cast<Asset>( std::make_shared<T>(url) );
		temp_sharedPtr->LoadAsset();
		// Add it to the library:
		this->allAssets[url] = temp_sharedPtr;					// Automatic conversion to weak pointer
		it = this->allAssets.find(url);

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nFound pre-loaded asset for url: %s", url.c_str());
	}
	ASSERT(it != this->allAssets.end(), "\nAsset could be found/loaded in AssetLibrary for url: %s", url.c_str());

	// TODO [Implement] Figure out how to ensure type safety here
	// ASSERT(it->second->GetAssetType() == T::assetType, "AssetType matches for asset found in library");
	return std::static_pointer_cast<T>((std::shared_ptr<Asset>)it->second);
}


#endif // ASSET_LIBRARY_H
