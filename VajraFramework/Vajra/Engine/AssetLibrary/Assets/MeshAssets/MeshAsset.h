#ifndef MESH_ASSET_H
#define MESH_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/Material/Material.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SimpleMesh.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>


class MeshAsset : public Asset, public SimpleMesh {
public:
	MeshAsset();
	MeshAsset(std::string urlOfMesh);
	virtual ~MeshAsset();

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();

	inline std::string GetShaderName() { return this->shaderName; }

	std::string GetFilePathToModel();

	void Draw();
	//
	void InitVerticesData(std::vector<glm::vec3> &inPositions, \
                          std::vector<glm::vec3> &inNormals,  \
                          std::vector<glm::vec2> &inTextureCoords);
	void InitIndicesData(std::vector<unsigned int> &inIndices);
	void InitBoneWeightInfluencesData(std::vector<glm::vec4> &inBoneIndices,
									  std::vector<glm::vec4> &inBoneWeights);
	//
	void MakeVBOs();

	inline glm::vec3 GetInitialPosition() const { return this->initialPosition; }
	inline glm::vec3 GetInitialRotation() const { return this->initialRotation; }
	inline glm::vec3 GetInitialScale()    const { return this->initialScale;    }

	inline std::string GetArmatureFilePath() { return this->armatureFilePath; }

	void SetGlRenderingMode(GLenum mode) { this->meshGlRenderingMode = mode; }

private:
	void init();
	void destroy();

	// For static render batching:
	inline int        getNumVertices()         { return this->numVertices;   }
	inline glm::vec3* getVertices()            { return this->vertices;      }
	inline glm::vec3* getNormals()             { return this->normals;       }
	inline glm::vec2* getTextureCoords()       { return this->textureCoords; }
	inline std::vector<unsigned int> getIndices() { return this->indices; }

	GLuint vboBoneIndices;
	GLuint vboBoneWeights;

	glm::vec4* boneIndices;		// Note: although this should technically be ivec4, OpenGL ES 2.0 doesn't support interger types in attributes, heh.
	glm::vec4* boneWeights;

	glm::vec3 initialPosition;
	glm::vec3 initialRotation;
	glm::vec3 initialScale;

	std::string armatureFilePath;

	Material* material;

	std::string shaderName;

	static AssetType assetType;

	friend class RenderBatch_static;
};

#endif // MESH_ASSET_H
