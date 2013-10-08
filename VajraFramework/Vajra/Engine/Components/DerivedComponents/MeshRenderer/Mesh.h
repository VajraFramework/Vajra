#ifndef MESH_H
#define MESH_H

#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef PLATFORM_DESKTOP
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
#endif // PLATFORM_DESKTOP

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // PLATFORM_IOS

#include "Libraries/glm/glm.hpp"

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

};

#endif // MESH_H