#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>

// Forward Declarations:
class ShaderSet;

class Material {
public:
	Material();
	~Material();

	inline glm::vec4 GetAmbientColor () { return this->ambientColor;  }
	inline glm::vec4 GetDiffuseColor () { return this->diffuseColor;  }
	inline glm::vec4 GetSpecularColor() { return this->specularColor; }
	//
	inline void SetAmbientColor (glm::vec4 color) { this->ambientColor  = color; }
	inline void SetDiffuseColor (glm::vec4 color) { this->diffuseColor  = color; }
	inline void SetSpecularColor(glm::vec4 color) { this->specularColor = color; }

	inline bool HasTexture()   { return (this->diffuseTextureAsset != nullptr); }
	inline bool HasNormalMap() { return (this->normalTextureAsset  != nullptr); }

	void SetTextureFilePath(std::string filePath);
	inline std::shared_ptr<TextureAsset>& GetTextureAsset() { return this->diffuseTextureAsset; }

	void SetNormalMapFilePath(std::string filePath);
	inline std::shared_ptr<TextureAsset>& GetNormalMapTextureAsset() { return this->normalTextureAsset; }

	void WriteMaterialToShader();

private:
	void init();
	void destroy();

	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;

	std::shared_ptr<TextureAsset> diffuseTextureAsset;
	std::shared_ptr<TextureAsset> normalTextureAsset;
};

#endif // MATERIAL_H
