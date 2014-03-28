#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/RenderBatch_static.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

RenderBatch_static::RenderBatch_static() : Renderer() {
	this->init();
}

RenderBatch_static::RenderBatch_static(Object* object_) : Renderer(object_) {
	this->init();
}

RenderBatch_static::~RenderBatch_static() {
	this->destroy();
}

void RenderBatch_static::HandleMessage(MessageChunk /* messageChunk */) {
}

void RenderBatch_static::CreateRenderBatchFromMeshes(std::vector<ObjectIdType> staticObjectIds) {
	VERIFY(!staticObjectIds.empty(), "Render batch not empty");
	FRAMEWORK->GetLogger()->dbglog("\nCreating a render batch out of %u meshes", staticObjectIds.size());

	ASSERT(this->vertices      == nullptr, "Render batch created only once");
	ASSERT(this->normals       == nullptr, "Render batch created only once");
	ASSERT(this->textureCoords == nullptr, "Render batch created only once");
	ASSERT(this->numVertices   == 0,       "Render batch created only once");

	// Add all the vertices in all the meshes that will be in the batch:
	for (ObjectIdType id : staticObjectIds) {
		GameObject* gameObject = this->gameobjectRef->GetParentSceneGraph()->GetGameObjectById(id);
		// TODO [Implement] Ensure type safety here:
		MeshRenderer* meshRenderer = gameObject->GetComponent<MeshRenderer>();
		MeshAsset* mesh = meshRenderer->GetMeshAsset();

		this->numVertices += mesh->getNumVertices();

		if (this->material == nullptr) {
			this->material = mesh->material;
		} else {
			// TODO [Implment] Check that all the meshes have the same material, and same texture, maybe
		}

		if (this->shaderName == "") {
			this->SetShaderName(mesh->GetShaderName());
		} else {
			VERIFY(this->GetShaderName() == mesh->GetShaderName(), "All meshes in the static render batch have the same shader");
		}

		VERIFY(mesh->getVertices() != nullptr, "Mesh has vertices");
		VERIFY(mesh->getNormals() != nullptr, "Mesh has normals");
		if (this->material->HasTexture()) {
			VERIFY(mesh->getTextureCoords() != nullptr, "Mesh has texture coords");
		}
		VERIFY(mesh->boneIndices == nullptr, "Mesh has no bones -- we don't support skinned animations in batched rendering yet");
		VERIFY(mesh->boneWeights == nullptr, "Mesh has no bones -- we don't support skinned animations in batched rendering yet");
	}

    this->vertices      = new glm::vec3[this->numVertices];
    this->normals       = new glm::vec3[this->numVertices];
	this->textureCoords = new glm::vec2[this->numVertices];


	/*
	 * Copy over vertex position, normals, and texcoords from the individual meshes in the batch,
	 * but bake them with the final model matrix.
	 * And copy over the indices as well.
	 */
	unsigned int vertexInBatch = 0;
	for (ObjectIdType id : staticObjectIds) {
		GameObject* gameObject = this->gameobjectRef->GetParentSceneGraph()->GetGameObjectById(id);
		// TODO [Implement] Ensure type safety here:
		MeshRenderer* meshRenderer = gameObject->GetComponent<MeshRenderer>();
		MeshAsset* mesh = meshRenderer->GetMeshAsset();

		unsigned int starting_vertex_of_mesh_in_batch = vertexInBatch;
		for (int i = 0; i < mesh->getNumVertices(); ++i, ++vertexInBatch) {
			// Bake the object's matrix into the vertex:
			glm::vec3 vertex = mesh->getVertices()[i];
			glm::vec4 baked_vertex4 = gameObject->GetTransform()->GetModelMatrixCumulative() * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
			this->vertices[vertexInBatch] = glm::vec3(baked_vertex4.x, baked_vertex4.y, baked_vertex4.z);
			//
			glm::vec3 normal = mesh->getNormals()[i];
			glm::vec4 baked_normal4 = gameObject->GetTransform()->GetModelMatrixCumulative() * glm::vec4(normal.x, normal.y, normal.z, 0.0f);
			this->normals[vertexInBatch]  = glm::vec3(baked_normal4.x, baked_normal4.y, baked_normal4.z);
			//
			if (this->material->HasTexture()) {
				this->textureCoords[vertexInBatch] = mesh->getTextureCoords()[i];
			}
		}

		std::vector<unsigned int> mesh_indices = mesh->getIndices();
		for (unsigned int index : mesh_indices) {
			this->indices.push_back(index + starting_vertex_of_mesh_in_batch);
		}
	}

#if 0
	/*
	 * Copy over indices from the individual meshes in the batch:
	 */
	for (ObjectIdType id : staticObjectIds) {
		GameObject* gameObject = this->gameobjectRef->GetParentSceneGraph()->GetGameObjectById(id);
		// TODO [Implement] Ensure type safety here:
		MeshRenderer* meshRenderer = gameObject->GetComponent<MeshRenderer>();
		MeshAsset* mesh = meshRenderer->GetMeshAsset();

		std::vector<unsigned int> mesh_indices = mesh->getIndices();
		for (unsigned int index : mesh_indices) {
			this->indices.push_back(index);
		}
	}
#endif

	// Done copying vertices data. Make VBOs:
	this->makeVBOs();

	Renderer::addToRenderList();
}

void RenderBatch_static::makeVBOs() {
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

void RenderBatch_static::Draw() {
    if (this->vboPositions == 0 || this->vboNormals == 0 || this->vboIndices == 0) {
        FRAMEWORK->GetLogger()->errlog("ERROR: VBOs not made");
        return;
    }
    if (this->vboTextureCoords == 0 && this->material->HasTexture()) {
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

    GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
    GLCALL(glDrawElements, this->meshGlRenderingMode, this->indices.size(), GL_UNSIGNED_INT, (void*)0);

    return;

}

void RenderBatch_static::init() {
	this->gameobjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameobjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->numVertices   = 0;
	this->vertices      = nullptr;
	this->normals       = nullptr;
	this->textureCoords = nullptr;
	this->material      = nullptr;

    this->meshGlRenderingMode = GL_TRIANGLES;
}

void RenderBatch_static::destroy() {
    if (this->vertices != nullptr) {
        delete this->vertices;
    }
    if (this->normals != nullptr) {
        delete this->normals;
    }
    if (this->textureCoords != nullptr) {
        delete this->textureCoords;
    }
    // TODO [Implement] Free the VBOs on RenderBatch_static::destroy()

}

