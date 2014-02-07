#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/ParticleSystemRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
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

void ParticleSystemRenderer::InitParticleSystem(std::string shaderName_) {
	this->SetShaderName(shaderName_);

	this->makeVBOs();

	// Now that we are renderable, add self to the render lists in the scene graph:
	// TODO [Implement] Ensure type safety here
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	ASSERT(gameObject != nullptr, "GameObject not null");
	ASSERT(gameObject->GetParentSceneGraph() != nullptr, "GameObject has been added to a scenegraph");
	gameObject->GetParentSceneGraph()->AddGameObjectToRenderLists(gameObject);
}

void ParticleSystemRenderer::HandleMessage(MessageChunk /* messageChunk */) {
}

void ParticleSystemRenderer::Draw() {
    if (this->vboPositions == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }

    ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

    GLint positionHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vPosition);
    glEnableVertexAttribArray(positionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
    glVertexAttribPointer(positionHandle,
                          3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError("glVertexAttribPointer");

    // Insert Texture here:

    glDrawArrays(GL_POINTS, 0, this->numVertices);
    checkGlError("glDrawArrays");
}

void ParticleSystemRenderer::makeVBOs() {
	// Temp, filling glm::vec3 vertices
	this->numVertices = 100;
	this->vertices = new glm::vec3[this->numVertices];
	for (unsigned int i = 0; i < this->numVertices; ++i) {
		this->vertices[i] = glm::vec3((float)i/10.0f, (float)i/10.0f, (float)i/10.0f);
	}
	//
	if (this->vertices != nullptr) {
		glGenBuffers(1, &this->vboPositions); checkGlError("glGenBuffers");
		glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions); checkGlError("glBindBuffer");
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->vertices, GL_STATIC_DRAW); checkGlError("glBufferData");
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
}

void ParticleSystemRenderer::destroy() {
}
