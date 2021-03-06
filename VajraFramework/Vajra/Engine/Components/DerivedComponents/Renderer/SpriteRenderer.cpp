#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

SpriteRenderer::SpriteRenderer() : Renderer() {
	this->init();
}

SpriteRenderer::SpriteRenderer(Object* object_) : Renderer(object_) {
	this->init();
}

SpriteRenderer::~SpriteRenderer() {
	this->destroy();
}

void SpriteRenderer::initPlane(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures, PlaneOrigin planeOrigin) {
	this->vertices = new glm::vec3[4];
	this->textureCoords = new glm::vec2[4];
	
	float halfWidth = (float)width * .5f;
	float halfHeight = (float)height * .5f;
	
	switch(planeOrigin) {
		case PlaneOrigin::Center:

			this->vertices[0] = glm::vec3(-halfWidth,  halfHeight, 0.0f);
			this->vertices[1] = glm::vec3(halfWidth, halfHeight, 0.0f);
			this->vertices[2] = glm::vec3(halfWidth, -1.0f * halfHeight, 0.0f);
			this->vertices[3] = glm::vec3(-halfWidth,  -1.0f * halfHeight, 0.0f);
			break;
		case PlaneOrigin::Bottom_Left:
			this->vertices[0] = glm::vec3(0.0f, 0.0f, 0.0f);
			this->vertices[1] = glm::vec3(width, 0.0f, 0.0f);
			this->vertices[2] = glm::vec3(width, -1.0f * height, 0.0f);
			this->vertices[3] = glm::vec3(0.0f,  -1.0f * height, 0.0f);
			break;
		default:
			ASSERT(0, "Implement support for other origins");
			break;
	}

	//
	this->textureCoords[0] = glm::vec2(0.0f, 1.0f);
	this->textureCoords[1] = glm::vec2(1.0f, 1.0f);
	this->textureCoords[2] = glm::vec2(1.0f, 0.0f);
	this->textureCoords[3] = glm::vec2(0.0f, 0.0f);
	//
	this->indices.clear();
	this->indices.push_back(0);
	this->indices.push_back(2);
	this->indices.push_back(1);
	this->indices.push_back(0);
	this->indices.push_back(3);
	this->indices.push_back(2);

	this->SetShaderName(shaderName_);

	this->initVbos();

	if (pathsToTextures.size() != 0) {
		for (std::string pathToTexture : pathsToTextures) {
			std::shared_ptr<TextureAsset> textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(pathToTexture);
			this->listOfTextureAssets.push_back(textureAsset);
		}
	}

	Renderer::addToRenderList();
}

void SpriteRenderer::initVbos() {
    if (this->vertices != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPositions);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, this->vertices, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }

    if (this->textureCoords != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboTextureCoords);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTextureCoords);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, this->textureCoords, GL_STATIC_DRAW);
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

void SpriteRenderer::HandleMessage(MessageChunk /* messageChunk */) {
}

void SpriteRenderer::Draw() {
	// Write Material properties to shader
	if (FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->HasHandle(SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor)) {
		GLCALL(glUniform4f, FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor), this->diffuseColor.r, this->diffuseColor.g, this->diffuseColor.b, this->diffuseColor.a);
	}

	if (this->vboPositions == 0 || this->vboIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    GLCALL(glEnableVertexAttribArray, positionHandle);
    GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
    GLCALL(glVertexAttribPointer, positionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (this->getNumberOfTextureAssets() != 0) {
    	this->getTextureAssetByIndex(this->currentTextureIndex)->Draw(0);

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

    GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
    GLCALL(glDrawElements, GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void SpriteRenderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->vertices = nullptr;
	this->textureCoords = nullptr;
	this->diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->currentTextureIndex = 0;
}

void SpriteRenderer::destroy() {
	// TODO [Hack] For some reason the simulator crashes in glDrawElements() when we do this
#ifdef __APPLE__
#if TARGET_IPHONE_SIMULATOR
	return;
#endif
#endif
	
	if (this->vertices != nullptr) {
		delete this->vertices;
	}
	if (this->textureCoords != nullptr) {
		delete this->textureCoords;
	}

    if (this->vboPositions != 0) {
    	FRAMEWORK->GetOpenGLWrapper()->FreeGLBuffer(&this->vboPositions);
    }
    if (this->vboTextureCoords != 0) {
    	FRAMEWORK->GetOpenGLWrapper()->FreeGLBuffer(&this->vboTextureCoords);
    }
    if (this->vboIndices != 0) {
    	FRAMEWORK->GetOpenGLWrapper()->FreeGLBuffer(&this->vboIndices);
    }
}
