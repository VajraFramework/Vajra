#ifndef MESH_H
#define MESH_H

#include "Vajra/Engine/AssetLibrary/Assets/TextureAsset.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>


class Mesh {
public:
	Mesh();
	~Mesh();

	void Draw();
	//
	void InitVerticesData(std::vector<glm::vec3> &inPositions, \
                          std::vector<glm::vec3> &inNormals,  \
                          std::vector<glm::vec2> &inTextureCoords);
	void InitIndicesData(std::vector<unsigned int> &inIndices);
	//
	void MakeVBOs();

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

	std::shared_ptr<TextureAsset> textureAsset;
};

#endif // MESH_H
