#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/ParticleSystemRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/ParticleSystems/Particle.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"

ParticleSystemRenderer::ParticleSystemRenderer() : Renderer() {
	this->init();
}

ParticleSystemRenderer::ParticleSystemRenderer(Object* object_) : Renderer(object_) {
	this->init();
}

ParticleSystemRenderer::~ParticleSystemRenderer() {
	this->destroy();
}

void ParticleSystemRenderer::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_PARTICLE_SYSTEM_INITED: {
		this->initializeRendererStructures();
	} break;

	case MESSAGE_TYPE_PARTICLE_SYSTEM_UPDATED: {
		this->updateVBOs();
	} break;

	default: {
	} break;

	}
}

void ParticleSystemRenderer::Draw() {
	if (!this->particleSystemRef->GetIsPlaying()) {
		return;
	}

	// TODO [Hack] Figure out if this can be done better. Disabling writing to the depth buffer to prevent particles from occluding each other incorrectly
	GLCALL(glDepthMask, false);

    if (this->vboPositions == 0 || this->vboPointSizes == 0 || this->vboParticleColors == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    if (this->particleSystemRef->inWorldSpace) {
    	static Transform* identity_transform = new Transform();
    	identity_transform->Draw();
    }

    {
    GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    GLCALL(glEnableVertexAttribArray, positionHandle);
    GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
    GLCALL(glVertexAttribPointer, positionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    {
    GLint pointsizesHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_pointSize);
    GLCALL(glEnableVertexAttribArray, pointsizesHandle);
    GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPointSizes);
    GLCALL(glVertexAttribPointer, pointsizesHandle, 1, GL_FLOAT, GL_FALSE, 0, 0);
    }

    {
    GLint particleColorHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_particleColor);
    GLCALL(glEnableVertexAttribArray, particleColorHandle);
    GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboParticleColors);
    GLCALL(glVertexAttribPointer, particleColorHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (this->textureAsset != nullptr) {
    	this->textureAsset->Draw(0);
    }

    GLCALL(glDrawArrays, GL_POINTS, 0, this->particleSystemRef->getNumParticlesToDraw());

	// TODO [Hack] Figure out if this can be done better:
	GLCALL(glDepthMask, true);
}

void ParticleSystemRenderer::initializeRendererStructures() {
	this->particleSystemRef = this->GetObject()->GetComponent<ParticleSystem>();
	VERIFY(this->particleSystemRef != nullptr, "Particle system found");

	// Make VBOs:
	if (this->particleSystemRef->getParticlePositionsForDrawing() != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPositions);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticlePositionsForDrawing(), GL_STATIC_DRAW);
	} else {
		FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
		return;
	}
	//
	if (this->particleSystemRef->getParticleSizesForDrawing() != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPointSizes);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPointSizes);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(float) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleSizesForDrawing(), GL_STATIC_DRAW);
	} else {
		FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
		return;
	}
	//
	if (this->particleSystemRef->getParticleColorsForDrawing() != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboParticleColors);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboParticleColors);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleColorsForDrawing(), GL_STATIC_DRAW);
	} else {
		FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
		return;
	}


	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(this->particleSystemRef->getPathToTexture());

	// TODO [Cleanup] Expose this, maybe
	// Set shader to use:
	this->SetShaderName("prtshdr");

	// Now that we are renderable, add self to the render lists in the scene graph:
	Renderer::addToRenderList();
}

void ParticleSystemRenderer::updateVBOs() {
	ASSERT(this->particleSystemRef != nullptr, "Particle system found");

	if (this->particleSystemRef->getParticlePositionsForDrawing() != nullptr) {
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticlePositionsForDrawing());
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleSizesForDrawing() != nullptr) {
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPointSizes);
		GLCALL(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(float) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleSizesForDrawing());
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleColorsForDrawing() != nullptr) {
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboParticleColors);
		GLCALL(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleColorsForDrawing());
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
}

void ParticleSystemRenderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->diffuseColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	this->particleSystemRef = nullptr;
	this->SetHasTransperancy(true);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_PARTICLE_SYSTEM_INITED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PARTICLE_SYSTEM_UPDATED, this->GetTypeId(), true);
}

void ParticleSystemRenderer::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
