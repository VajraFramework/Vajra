#ifndef MESH_ASSET_H
#define MESH_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAsset.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>


class MeshAsset : public Asset {
public:
	MeshAsset();
	MeshAsset(std::string urlOfMesh);
	~MeshAsset();

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();

	std::string GetFilePathToModel();

	void Draw();
	//
	void InitVerticesData(std::vector<glm::vec3> &inPositions, \
                          std::vector<glm::vec3> &inNormals,  \
                          std::vector<glm::vec2> &inTextureCoords);
	void InitIndicesData(std::vector<unsigned int> &inIndices);
	//
	void MakeVBOs();

	inline glm::vec3 GetInitialPosition() const { return this->initialPosition; }
	inline glm::vec3 GetInitialRotation() const { return this->initialRotation; }
	inline glm::vec3 GetInitialScale()    const { return this->initialScale;    }

	void SetTextureFilePath(std::string filePath);

private:
	void init();
	void destroy();


	GLuint vboPositions;
	GLuint vboNormals;
	GLuint vboTextureCoords;
	//
	GLuint vboIndices;

	int numVertices;

	glm::vec3 *vertices;
	glm::vec3 *normals;
	glm::vec2 *textureCoords;
	//
	std::vector<unsigned int> indices;

	glm::vec3 initialPosition;
	glm::vec3 initialRotation;
	glm::vec3 initialScale;

	std::shared_ptr<TextureAsset> textureAsset;

	static AssetType assetType;
};

#endif // MESH_ASSET_H
