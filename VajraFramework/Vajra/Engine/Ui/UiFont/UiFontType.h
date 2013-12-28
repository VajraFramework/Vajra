#ifndef UIFONTTYPE_H
#define UIFONTTYPE_H

#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"
#include "Vajra/Framework/Loaders/FontLoader/Declarations.h"

#include <memory>
#include <string>
#include <vector>

struct FontSheet {
public:
private:
	void loadToFontSheetTexture(std::string pathToTexture);

	float width, height;
	std::string pathToFontSheetTexture;
	std::shared_ptr<TextureAsset> textureAsset;

	friend class UiFontType;
};

struct FontSpecification {
public:
private:
	FontSpecification() {
		this->characterWidths.resize(NUM_FONT_CHARACTER_ROWS * NUM_FONT_CHARACTER_COLUMNS, 0.0f);
	}

	float fontboxWidth, fontboxHeight;
	std::vector<float> characterWidths;
	int startingCharacter;

	friend class UiFontType;
};

class UiFontType {
public:
	UiFontType(std::string pathToFontSheetTexture, std::string pathToFontSpecificationCSV, std::string shaderName_);
	~UiFontType();

	inline std::string GetShaderName() { return this->shaderName; }

	inline std::shared_ptr<TextureAsset>& GetTextureAsset() { return this->fontSheet.textureAsset; }

	inline float GetCharacterWidth(int charIdx) { return this->fontSpecification.characterWidths[charIdx]; }
	inline int GetStartingCharacter() { return this->fontSpecification.startingCharacter; }

private:
	FontSheet fontSheet;
	FontSpecification fontSpecification;
	std::string shaderName;
};

#endif // UIFONTTYPE_H
