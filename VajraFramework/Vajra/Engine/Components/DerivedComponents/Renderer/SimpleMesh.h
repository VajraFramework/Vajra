#ifndef SIMPLE_MESH_H
#define SIMPLE_MESH_H

#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/Material/Material.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"

#include <string>
#include <vector>

class SimpleMesh {
public:
	SimpleMesh();
	~SimpleMesh();

protected:
	GLuint vboPositions;
	GLuint vboNormals;
	GLuint vboTangents;
	GLuint vboBitangents;
	GLuint vboTextureCoords;
	//
	GLuint vboIndices;

	int numVertices;

	glm::vec3* vertices;
	glm::vec3* normals;
	glm::vec3* tangents;
	glm::vec3* bitangents;
	glm::vec2* textureCoords;
	//
	std::vector<unsigned int> indices;

	Material* materialRef;

	GLenum meshGlRenderingMode;

	void makeVBOs();

	void prepareVerticesToDraw();
	void draw();

private:
	void init();
	void destroy();

	void computeTangentsAndBitangents();

};

#endif // SIMPLE_MESH_H
