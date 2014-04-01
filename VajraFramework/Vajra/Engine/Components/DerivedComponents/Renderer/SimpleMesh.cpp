#include "Vajra/Engine/Components/DerivedComponents/Renderer/SimpleMesh.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

SimpleMesh::SimpleMesh() {
	this->init();
}

SimpleMesh::~SimpleMesh() {
	this->destroy();
}

void SimpleMesh::init() {
	this->numVertices = 0;

    this->vertices = nullptr;
    this->normals = nullptr;
    this->textureCoords = nullptr;

    this->meshGlRenderingMode = GL_TRIANGLES;

    this->materialRef = nullptr;

    this->vboPositions = 0;
    this->vboNormals = 0;
    this->vboTextureCoords = 0;
    this->vboIndices = 0;
}

void SimpleMesh::makeVBOs() {
    if (this->vertices != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPositions);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->vertices, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }

    if (this->normals != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboNormals);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboNormals);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->normals, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited normals");
        return;
    }

    if (this->textureCoords != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboTextureCoords);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTextureCoords);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->numVertices, this->textureCoords, GL_STATIC_DRAW);
    } else {
        ASSERT(!this->materialRef->HasTexture(), "Texture coords missing because model has no texture");
    }

    if (this->indices.size() != 0) {
		GLCALL(glGenBuffers, 1, &this->vboIndices);
		GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
		GLCALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited indices");
        return;
    }
}

void SimpleMesh::prepareVerticesToDraw() {

    if (this->vboPositions == 0 || this->vboNormals == 0 || this->vboIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }
    if (this->vboTextureCoords == 0 && this->materialRef->HasTexture()) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Texture VBOs not made");
        return;
    }

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    GLCALL(glEnableVertexAttribArray, positionHandle);
    GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
    GLCALL(glVertexAttribPointer, positionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //
    // TODO [Hack] Do this better, maybe:
    // No normals in depth pass:
    if (!currentShaderSet->IsDepthPass()) {
    	GLint normalHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vNormal);
    	GLCALL(glEnableVertexAttribArray, normalHandle);
    	GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboNormals);
    	GLCALL(glVertexAttribPointer, normalHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //
    // TODO [Hack] Do this better, maybe:
    // No materials in depth pass:
    if (!currentShaderSet->IsDepthPass()) {
    	this->materialRef->WriteMaterialToShader();
    	if (this->materialRef->HasTexture()) {
    		this->materialRef->GetTextureAsset()->Draw(0);

			GLint textureCoordsHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in);
    		GLCALL(glEnableVertexAttribArray, textureCoordsHandle);
    		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTextureCoords);
    		GLCALL(glVertexAttribPointer, textureCoordsHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);

    	} else {
    		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in)) {
				GLint textureCoordsHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in);
				GLCALL(glDisableVertexAttribArray, textureCoordsHandle);
    		}
    	}
    }
}

void SimpleMesh::draw() {

    GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
    GLCALL(glDrawElements, this->meshGlRenderingMode, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
}


void SimpleMesh::destroy() {
    if (this->vertices != nullptr) {
        delete this->vertices;
    }
    if (this->normals != nullptr) {
        delete this->normals;
    }
    if (this->textureCoords != nullptr) {
        delete this->textureCoords;
    }

    if (this->vboPositions != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboPositions);
    }
    if (this->vboNormals != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboNormals);
    }
    if (this->vboTextureCoords != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboTextureCoords);
    }
    if (this->vboIndices != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboIndices);
    }
}
