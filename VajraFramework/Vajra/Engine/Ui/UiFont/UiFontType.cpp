#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Ui/UiFont/UiFontType.h"
#include "Vajra/Framework/Loaders/FontLoader/FontLoader.h"

UiFontType::UiFontType(std::string pathToFontSheetTexture, std::string pathToFontSpecificationCSV, std::string shaderName_) {
	this->shaderName = shaderName_;

	FontLoader::LoadFontSpecificationsFromCSVFile(pathToFontSpecificationCSV.c_str(),
												  this->fontSheet.width, this->fontSheet.height,
												  this->fontSpecification.fontboxWidth, this->fontSpecification.fontboxHeight,
												  this->fontSpecification.startingCharacter,
												  this->fontSpecification.characterWidths);
	this->fontSheet.loadToFontSheetTexture(pathToFontSheetTexture);


}

UiFontType::~UiFontType() {
}

void FontSheet::loadToFontSheetTexture(std::string pathToTexture) {
	this->pathToFontSheetTexture = pathToTexture;
	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(pathToTexture);
}
