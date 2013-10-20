#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"

AssetLibrary::AssetLibrary() {
	this->init();
}

AssetLibrary::~AssetLibrary() {
	this->destroy();
}

void AssetLibrary::init() {
}

void AssetLibrary::destroy() {
	this->allAssets.clear();
}


