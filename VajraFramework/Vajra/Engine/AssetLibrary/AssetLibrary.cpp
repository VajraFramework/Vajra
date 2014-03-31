#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"

AssetLibrary::AssetLibrary() {
}

AssetLibrary::~AssetLibrary() {
	this->destroy();
}

void AssetLibrary::init() {
}

void AssetLibrary::destroy() {
	this->allAssets.clear();
}


