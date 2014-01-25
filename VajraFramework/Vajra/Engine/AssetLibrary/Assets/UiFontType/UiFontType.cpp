#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/UiFontType/UiFontType.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Loaders/FontLoader/FontLoader.h"

AssetType UiFontType::assetType = ASSET_TYPE_FONTTYPE;

UiFontType::UiFontType(std::string urlOfFontSpecificationFile) : Asset(urlOfFontSpecificationFile) {
	this->init();
}

UiFontType::~UiFontType() {
	this->destroy();
}

void FontSheet::loadToFontSheetTexture(std::string pathToTexture) {
	this->pathToFontSheetTexture = pathToTexture;
	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(pathToTexture);
}

std::string UiFontType::GetFilePathToFontSpecificationFile() {
	return this->GetUrl();
}

void UiFontType::loadFont_internal(std::string nameOfFontSheetTexture, std::string pathToFontSpecificationCSV, std::string shaderName_) {
	this->shaderName = shaderName_;

	FontLoader::LoadFontSpecificationsFromCSVFile(pathToFontSpecificationCSV.c_str(),
												  this->fontSheet.width, this->fontSheet.height,
												  this->fontSpecification.fontboxWidth, this->fontSpecification.fontboxHeight,
												  this->fontSpecification.startingCharacter,
												  this->fontSpecification.characterWidths);
	this->fontSheet.loadToFontSheetTexture(nameOfFontSheetTexture);


}

AssetType UiFontType::GetAssetType() {
	return UiFontType::assetType;
}

void UiFontType::LoadAsset() {
	ASSERT(this->GetUrl() != "", "Url set when trying to font type asset");

	FRAMEWORK->GetLogger()->dbglog("\nLoading font type asset from url: %s", this->GetFilePathToFontSpecificationFile().c_str());

	std::ifstream fontSpecificationFile(this->GetFilePathToFontSpecificationFile().c_str());
	ASSERT(fontSpecificationFile.good(), "Successfully opened font type specification file: %s", this->GetFilePathToFontSpecificationFile().c_str());

	std::string nameOfFontSheetTexture;
	std::string nameOfFontSpecificationCSV;
	std::string shaderName;
	fontSpecificationFile >> nameOfFontSheetTexture;
	fontSpecificationFile >> nameOfFontSpecificationCSV;
	fontSpecificationFile >> shaderName;

	this->loadFont_internal(FRAMEWORK->GetFileSystemUtils()->GetDeviceFontResourcesFolderName() + nameOfFontSheetTexture,
							FRAMEWORK->GetFileSystemUtils()->GetDeviceFontResourcesPath() + nameOfFontSpecificationCSV,
							shaderName);
}

void UiFontType::init() {
}

void UiFontType::destroy() {
}
