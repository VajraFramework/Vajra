#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Loaders/ModelLoader/ModelLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

#define NUM_VERTS_PER_FACE 3

AssetType MeshAsset::assetType = ASSET_TYPE_MESH_DATA;

MeshAsset::MeshAsset() : Asset() {
    this->init();
}

MeshAsset::MeshAsset(std::string urlOfMesh) : Asset(urlOfMesh) {
	this->init();
}

MeshAsset::~MeshAsset() {
    this->destroy();
}

AssetType MeshAsset::GetAssetType() {
	return MeshAsset::assetType;
}

void MeshAsset::LoadAsset() {
	// Stuff to initialize mesh data with:
	std::vector<glm::vec3> out_meshPositions;
	std::vector<glm::vec3> out_meshNormals;
	std::vector<glm::vec2> out_meshTextureCoords;
	//
	std::vector<glm::vec4> out_meshBoneIndices;
	std::vector<glm::vec4> out_meshBoneWeights;
	//
	std::vector<unsigned int> out_meshIndices;
	//
	glm::vec3 out_initialPosition;
	glm::vec3 out_initialRotation;
	glm::vec3 out_initialScale;
	//
	glm::vec4 out_ambientColor;
	glm::vec4 out_diffuseColor;
	glm::vec4 out_specularColor;
	std::string out_textureFileName;
	//
	std::string out_armatureFilePath;
	//
	std::string out_shaderName;

	ModelLoader::LoadMeshFromModelFile(this->GetFilePathToModel().c_str(), out_meshPositions, out_meshNormals, out_meshTextureCoords, out_meshBoneIndices, out_meshBoneWeights, out_meshIndices, out_initialPosition, out_initialRotation, out_initialScale, out_ambientColor, out_diffuseColor, out_specularColor, out_textureFileName, out_armatureFilePath, out_shaderName);

	this->InitVerticesData(out_meshPositions, out_meshNormals, out_meshTextureCoords);
	this->InitIndicesData(out_meshIndices);
	if (out_meshBoneIndices.size() != 0 && out_meshBoneWeights.size() != 0) {
		this->InitBoneWeightInfluencesData(out_meshBoneIndices, out_meshBoneWeights);
	}

	this->initialPosition = out_initialPosition;
	this->initialRotation = out_initialRotation;
	this->initialScale    = out_initialScale;

	this->material = new Material();
	this->material->SetAmbientColor(out_ambientColor);
	this->material->SetDiffuseColor(out_diffuseColor);
	this->material->SetSpecularColor(out_specularColor);
	if (out_textureFileName != "") {
		this->material->SetTextureFilePath(out_textureFileName);
	}

	if (out_armatureFilePath != "") {
		this->armatureFilePath = out_armatureFilePath;
	}

	this->shaderName = out_shaderName;

	this->MakeVBOs();
}

std::string MeshAsset::GetFilePathToModel() {
	return this->GetUrl();
}

void MeshAsset::InitVerticesData(std::vector<glm::vec3> &inPositions, \
                             std::vector<glm::vec3> &inNormals, \
                             std::vector<glm::vec2> &inTextureCoords) {

    if (inPositions.size() != inNormals.size() ||
        (inTextureCoords.size() != 0  &&  inPositions.size() != inTextureCoords.size())) {

        FRAMEWORK->GetLogger()->errlog("ERROR: Unequal numbers of vertices, normals and textureCoords passed: \
                             %d, %d, %d", inPositions.size(), inNormals.size(), inTextureCoords.size());
        return;
    }

    this->numVertices = inPositions.size();
    if (this->vertices != nullptr) {
        delete[] this->vertices;
    }
    if (this->normals != nullptr) {
        delete[] this->normals;
    }
    if (this->textureCoords != nullptr) {
        delete[] this->textureCoords;
    }
    this->vertices      = new glm::vec3[this->numVertices];
    this->normals       = new glm::vec3[this->numVertices];
    if (inTextureCoords.size() != 0) {
		this->textureCoords = new glm::vec2[this->numVertices];
    }

    for (int i = 0; i < this->numVertices; ++i) {
        this->vertices[i].x = inPositions[i].x;
        this->vertices[i].y = inPositions[i].y;
        this->vertices[i].z = inPositions[i].z;
        //
        this->normals[i].x = inNormals[i].x;
        this->normals[i].y = inNormals[i].y;
        this->normals[i].z = inNormals[i].z;
    }
    //
    if (inTextureCoords.size() != 0) {
		for (int i = 0; i < this->numVertices; ++i) {
			this->textureCoords[i].x = inTextureCoords[i].x;
			this->textureCoords[i].y = inTextureCoords[i].y;
		}
    }

    return;
}

void MeshAsset::InitIndicesData(std::vector<unsigned int> &inIndices) {
    this->indices.clear();

    for (unsigned int i = 0; i < inIndices.size(); ++i) {
        this->indices.push_back(inIndices[i]);
    }

    return;
}

void MeshAsset::InitBoneWeightInfluencesData(
		std::vector<glm::vec4>& inBoneIndices,
		std::vector<glm::vec4>& inBoneWeights) {

	if ((unsigned int)this->numVertices != inBoneIndices.size() || (unsigned int)this->numVertices != inBoneWeights.size()) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Unequal numbers of vertices and bone weights passed: \
                             %d, %d, %d", this->numVertices, inBoneIndices.size(), inBoneWeights.size());
	}

    if (this->boneIndices != nullptr) {
        delete[] this->boneIndices;
    }
    if (this->boneWeights != nullptr) {
        delete[] this->boneWeights;
    }

    this->boneIndices      = new glm::vec4[this->numVertices];
    this->boneWeights      = new glm::vec4[this->numVertices];

    for (int i = 0; i < this->numVertices; ++i) {
    	this->boneIndices[i] = inBoneIndices[i];
    	this->boneWeights[i] = inBoneWeights[i];
    }

    return;
}


void MeshAsset::MakeVBOs() {
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
        ASSERT(!this->material->HasTexture(), "Texture coords missing because model has no texture");
    }

    if (this->boneIndices != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboBoneIndices);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneIndices);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->numVertices, this->boneIndices, GL_STATIC_DRAW);
    }

    if (this->boneWeights != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboBoneWeights);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneWeights);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->numVertices, this->boneWeights, GL_STATIC_DRAW);
    }

    if (this->indices.size() != 0) {
		GLCALL(glGenBuffers, 1, &this->vboIndices);
		GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
		GLCALL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited indices");
        return;
    }

    FRAMEWORK->GetLogger()->errlog("\nVBOs made successfully");
}

void MeshAsset::Draw() {
    if (this->vboPositions == 0 || this->vboNormals == 0 || this->vboIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }
    if (this->vboTextureCoords == 0 && this->material->HasTexture()) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Texture VBOs not made");
        return;
    }
    if (this->boneIndices != nullptr && this->vboBoneIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Bone indices VBOs not made");
        return;
    }
    if (this->boneWeights != nullptr && this->vboBoneWeights == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Bone weights VBOs not made");
        return;
    }

    // GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
    // GLCALL(glEnableClientState, GL_VERTEX_ARRAY);
    // GLCALL(glVertexPointer, 3, GL_FLOAT, 0, 0);

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
    	this->material->WriteMaterialToShader();
    	if (this->material->HasTexture()) {
    		this->material->GetTextureAsset()->Draw(0);

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
    //
    if (this->boneIndices != nullptr) {
		GLint boneIndicesHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneIndices);
		GLCALL(glEnableVertexAttribArray, boneIndicesHandle);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneIndices);
		GLCALL(glVertexAttribPointer, boneIndicesHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //
    if (this->boneWeights != nullptr) {
		GLint boneWeightsHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneWeights);
		GLCALL(glEnableVertexAttribArray, boneWeightsHandle);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneWeights);
		GLCALL(glVertexAttribPointer, boneWeightsHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //
    GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
    GLCALL(glDrawElements, this->meshGlRenderingMode, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

    return;
}


void MeshAsset::init() {
    this->vertices = nullptr;
    this->normals = nullptr;
    this->textureCoords = nullptr;
    this->material = nullptr;
    //
    this->boneIndices = nullptr;
    this->boneWeights = nullptr;

    this->meshGlRenderingMode = GL_TRIANGLES;
}

void MeshAsset::destroy() {
    if (this->vertices != nullptr) {
        delete this->vertices;
    }
    if (this->normals != nullptr) {
        delete this->normals;
    }
    if (this->textureCoords != nullptr) {
        delete this->textureCoords;
    }
    if (this->boneIndices != nullptr) {
        delete this->boneIndices;
    }
    if (this->boneWeights != nullptr) {
        delete this->boneWeights;
    }
    // TODO [Implement] Free the VBOs on MeshAsset::destroy()

}
