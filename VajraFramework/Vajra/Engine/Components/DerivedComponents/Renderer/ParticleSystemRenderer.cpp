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
	// TODO [Hack] Figure out if this can be done better. Disabling writing to the depth buffer to prevent particles from occluding each other incorrectly
	glDepthMask(false);

    if (this->vboPositions == 0 || this->vboPointSizes == 0 || this->vboParticleColors == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    {
    GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    glEnableVertexAttribArray(positionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
    glVertexAttribPointer(positionHandle,
                          3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
    }

    {
    GLint pointsizesHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_pointSize);
    glEnableVertexAttribArray(pointsizesHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPointSizes); checkGlError("glBindBuffer");
    glVertexAttribPointer(pointsizesHandle,
                          1, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
    }

    {
    GLint particleColorHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_particleColor);
    glEnableVertexAttribArray(particleColorHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboParticleColors); checkGlError("glBindBuffer");
    glVertexAttribPointer(particleColorHandle,
                          4, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");
    }

	glBindTexture(GL_TEXTURE_2D, this->textureAsset->GetGLTextureHandle());
	checkGlError("glBindTexture");

    glDrawArrays(GL_POINTS, 0, this->particleSystemRef->getNumParticlesToDraw());
    checkGlError("glDrawArrays");

	// TODO [Hack] Figure out if this can be done better:
	glDepthMask(true);
}

void ParticleSystemRenderer::initializeRendererStructures() {
	this->particleSystemRef = this->GetObject()->GetComponent<ParticleSystem>();
	VERIFY(this->particleSystemRef != nullptr, "Particle system found");

	// Make VBOs:
	if (this->particleSystemRef->getParticlePositionsForDrawing() != nullptr) {
		glGenBuffers(1, &this->vboPositions); checkGlError("glGenBuffers");
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticlePositionsForDrawing(), GL_STATIC_DRAW); checkGlError("glBufferData");
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleSizesForDrawing() != nullptr) {
		glGenBuffers(1, &this->vboPointSizes); checkGlError("glGenBuffers");
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPointSizes); checkGlError("glBindBuffer");
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleSizesForDrawing(), GL_STATIC_DRAW); checkGlError("glBufferData");
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleColorsForDrawing() != nullptr) {
		glGenBuffers(1, &this->vboParticleColors); checkGlError("glGenBuffers");
		glBindBuffer(GL_ARRAY_BUFFER, this->vboParticleColors); checkGlError("glBindBuffer");
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleColorsForDrawing(), GL_STATIC_DRAW); checkGlError("glBufferData");
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }


	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(this->particleSystemRef->getPathToTexture());

	// TODO [Cleanup] Expose this, maybe
	// Set shader to use:
	this->SetShaderName("prtshdr");

	// Now that we are renderable, add self to the render lists in the scene graph:
	// TODO [Implement] Ensure type safety here
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	ASSERT(gameObject != nullptr, "GameObject not null");
	ASSERT(gameObject->GetParentSceneGraph() != nullptr, "GameObject has been added to a scenegraph");
	gameObject->GetParentSceneGraph()->AddGameObjectToRenderLists(gameObject);
}

void ParticleSystemRenderer::updateVBOs() {
	ASSERT(this->particleSystemRef != nullptr, "Particle system found");

	if (this->particleSystemRef->getParticlePositionsForDrawing() != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticlePositionsForDrawing()); checkGlError("glBufferSubData");
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleSizesForDrawing() != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPointSizes); checkGlError("glBindBuffer");
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleSizesForDrawing()); checkGlError("glBufferSubData");
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }
	//
	if (this->particleSystemRef->getParticleColorsForDrawing() != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, this->vboParticleColors); checkGlError("glBindBuffer");
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * this->particleSystemRef->getNumParticlesToDraw(), this->particleSystemRef->getParticleColorsForDrawing()); checkGlError("glBufferSubData");
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

	this->addSubscriptionToMessageType(MESSAGE_TYPE_PARTICLE_SYSTEM_INITED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PARTICLE_SYSTEM_UPDATED, this->GetTypeId(), true);
}

void ParticleSystemRenderer::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
