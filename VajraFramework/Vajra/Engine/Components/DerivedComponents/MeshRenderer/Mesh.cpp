#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

#define NUM_VERTS_PER_FACE 3

Mesh::Mesh() {
    this->init();
}

Mesh::~Mesh() {
    this->destroy();
}

void Mesh::InitVerticesData(std::vector<glm::vec3> &inPositions, \
                             std::vector<glm::vec3> &inNormals, \
                             std::vector<glm::vec2> &inTextureCoords) {

    if (inPositions.size() != inNormals.size() ||
        inPositions.size() != inTextureCoords.size()) {

        FRAMEWORK->GetLogger()->errlog("ERROR: Unequal numbers of vertices, normals and textureCoords passed: \
                             %d, %d, %d", inPositions.size(), inNormals.size(), inTextureCoords.size());
        return;
    }

    this->numVertices = inPositions.size();
    if (this->vertices != 0) {
        delete[] this->vertices;
    }
    if (this->normals != 0) {
        delete[] this->normals;
    }
    if (this->textureCoords != 0) {
        delete[] this->textureCoords;
    }
    this->vertices      = new glm::vec3[this->numVertices];
    this->normals       = new glm::vec3[this->numVertices];
    this->textureCoords = new glm::vec2[this->numVertices];

    for (int i = 0; i < this->numVertices; ++i) {
        this->vertices[i].x = inPositions[i].x;
        this->vertices[i].y = inPositions[i].y;
        this->vertices[i].z = inPositions[i].z;
        //
        this->normals[i].x = inNormals[i].x;
        this->normals[i].y = inNormals[i].y;
        this->normals[i].z = inNormals[i].z;
        //
        this->textureCoords[i].x = inTextureCoords[i].x;
        this->textureCoords[i].y = inTextureCoords[i].y;
    }

    return;
}

void Mesh::InitIndicesData(std::vector<unsigned int> &inIndices) {
    this->indices.clear();

    for (unsigned int i = 0; i < inIndices.size(); ++i) {
        this->indices.push_back(inIndices[i]);
    }

    return;
}

void Mesh::MakeVBOs() {
    if (this->vertices == 0 || this->normals == 0 || this->textureCoords == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices, or normals or textureCoords");
        return;
    }
    if (this->indices.size() == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited indices");
        return;
    }

    glGenBuffers(1, &this->vboPositions); checkGlError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->vertices, GL_STATIC_DRAW); checkGlError("glBufferData");

    glGenBuffers(1, &this->vboNormals); checkGlError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboNormals); checkGlError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->normals, GL_STATIC_DRAW); checkGlError("glBufferData");

    glGenBuffers(1, &this->vboTextureCoords); checkGlError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureCoords); checkGlError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->numVertices, this->textureCoords, GL_STATIC_DRAW); checkGlError("glBufferData");

    glGenBuffers(1, &this->vboIndices); checkGlError("glGenBuffers");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIndices); checkGlError("glBindBuffer");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW); checkGlError("glBufferData");

    FRAMEWORK->GetLogger()->errlog("\nVBOs made successfully");
}

void Mesh::SetTextureFilePath(std::string filePath) {
	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(filePath);
}

void Mesh::Draw() {
    if (this->vboPositions == 0 || this->vboNormals == 0 || this->vboTextureCoords == 0 || this->vboIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }

    // glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions);
    // glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(3, GL_FLOAT, 0, 0);

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    glUseProgram(currentShaderSet->GetShaderProgram());
    checkGlError("glUseProgram");

    glEnableVertexAttribArray(currentShaderSet->GetPositionHandle());
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
    glVertexAttribPointer(currentShaderSet->GetPositionHandle(), \
                          3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
    //
    #if 0
    // no normals yet
    glEnableVertexAttribArray(currentShaderSet->GetNormalsHandle());
    glBindBuffer(GL_ARRAY_BUFFER, this->vboNormals); checkGlError("glBindBuffer");
    glVertexAttribPointer(currentShaderSet->GetPositionHandle(), \
                          3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
    #endif // 0 no normals yet
    //
    if (this->textureAsset) {
    	glBindTexture(GL_TEXTURE_2D, this->textureAsset->GetGLTextureHandle());
    	checkGlError("glBindTexture");

    	glEnableVertexAttribArray(currentShaderSet->GetTextureCoordsHandle());
    	glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureCoords); checkGlError("glBindBuffer");
    	glVertexAttribPointer(currentShaderSet->GetTextureCoordsHandle(), \
    			2, GL_FLOAT, GL_FALSE, 0, 0);
    	checkGlError("glVertexAttribPointer");
    } else {
    	glDisableVertexAttribArray(currentShaderSet->GetTextureCoordsHandle());
    }
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIndices); checkGlError("glBindBuffer");
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
    checkGlError("glDrawElements");

    return;
}


void Mesh::init() {
    this->vertices = 0;
    this->normals = 0;
    this->textureCoords = 0;
}

void Mesh::destroy() {
    if (this->vertices != 0) {
        delete this->vertices;
    }
    if (this->normals != 0) {
        delete this->normals;
    }
    if (this->textureCoords != 0) {
        delete this->textureCoords;
    }
    // TODO [Implement] Free the VBOs on Mesh::destroy()
}
