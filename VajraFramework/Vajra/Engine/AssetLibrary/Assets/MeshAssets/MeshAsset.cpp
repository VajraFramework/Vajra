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
	std::string out_normalMapFileName;
	//
	std::string out_armatureFilePath;
	//
	std::string out_shaderName;

	ModelLoader::LoadMeshFromModelFile(this->GetFilePathToModel().c_str(), out_meshPositions, out_meshNormals, out_meshTextureCoords, out_meshBoneIndices, out_meshBoneWeights, out_meshIndices, out_initialPosition, out_initialRotation, out_initialScale, out_ambientColor, out_diffuseColor, out_specularColor, out_textureFileName, out_normalMapFileName, out_armatureFilePath, out_shaderName);

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
	if (out_textureFileName != "") {
		this->material->SetTextureFilePath(out_textureFileName);
	}
	if (out_normalMapFileName != "") {
		this->material->SetNormalMapFilePath(out_normalMapFileName);
	}
	SimpleMesh::materialRef = this->material;

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
	SimpleMesh::makeVBOs();

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

    FRAMEWORK->GetLogger()->errlog("\nVBOs made successfully");
}

void MeshAsset::Draw() {
	SimpleMesh::prepareVerticesToDraw();

    if (this->boneIndices != nullptr && this->vboBoneIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Bone indices VBOs not made");
        return;
    }
    if (this->boneWeights != nullptr && this->vboBoneWeights == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: Bone weights VBOs not made");
        return;
    }

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    if (this->vboBoneIndices != 0) {
		GLint boneIndicesHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneIndices);
		GLCALL(glEnableVertexAttribArray, boneIndicesHandle);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneIndices);
		GLCALL(glVertexAttribPointer, boneIndicesHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //
    if (this->vboBoneWeights != 0) {
		GLint boneWeightsHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneWeights);
		GLCALL(glEnableVertexAttribArray, boneWeightsHandle);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBoneWeights);
		GLCALL(glVertexAttribPointer, boneWeightsHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

	SimpleMesh::draw();

    return;
}

void MeshAsset::init() {
    this->boneIndices = nullptr;
    this->boneWeights = nullptr;

    this->vboBoneIndices = 0;
    this->vboBoneWeights = 0;
}

void MeshAsset::destroy() {
    if (this->boneIndices != nullptr) {
        delete this->boneIndices;
    }
    if (this->boneWeights != nullptr) {
        delete this->boneWeights;
    }

    if (this->vboBoneIndices != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboBoneIndices);
    }
    if (this->vboBoneWeights != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboBoneWeights);
    }

    delete this->material;
}
