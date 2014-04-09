#include "Vajra/Engine/Components/DerivedComponents/Renderer/SimpleMesh.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
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
    this->tangents = nullptr;
    this->bitangents = nullptr;
    this->textureCoords = nullptr;

    this->meshGlRenderingMode = GL_TRIANGLES;

    this->materialRef = nullptr;

    this->vboPositions = 0;
    this->vboNormals = 0;
    this->vboTangents = 0;
    this->vboBitangents = 0;
    this->vboTextureCoords = 0;
    this->vboIndices = 0;
}

void SimpleMesh::computeTangentsAndBitangents() {
	if (this->textureCoords == nullptr) {
		// No texture coords, nothing to be done
		return;
	}

	ASSERT(this->tangents == nullptr, "Tangents not already computed");
	ASSERT(this->bitangents == nullptr, "Bitangents not already computed");

	this->tangents   = new glm::vec3[this->numVertices];
	this->bitangents = new glm::vec3[this->numVertices];

	for (int i = 0; i < this->numVertices; ++i) {
		this->tangents[i]   = glm::vec3(0.0f, 0.0f, 0.0f);
		this->bitangents[i] = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	ASSERT((this->indices.size() % 3 == 0), "Number of indices (%u) is a multiple of 3", this->indices.size());


	for (unsigned int face = 0; face < (this->indices.size() / 3); ++face) {
		unsigned int i0 = this->indices[face * 3 + 0];
		unsigned int i1 = this->indices[face * 3 + 1];
		unsigned int i2 = this->indices[face * 3 + 2];

		// Do not process degenerate triangles:
		if (i0 == i1 || i0 == i2 || i1 == i2) {
			continue;
		}

		// Shortcuts for vertices
		glm::vec3 & v0 = this->vertices[i0];
		glm::vec3 & v1 = this->vertices[i1];
		glm::vec3 & v2 = this->vertices[i2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = this->textureCoords[i0];
		glm::vec2 & uv1 = this->textureCoords[i1];
		glm::vec2 & uv2 = this->textureCoords[i2];

		// Edges of the triangle : postion delta
		glm::vec3 Edge1 = v1-v0;
		glm::vec3 Edge2 = v2-v0;

		// UV delta
		float DeltaU1 = uv1.x - uv0.x;
		float DeltaV1 = uv1.y - uv0.y;
		float DeltaU2 = uv2.x - uv0.x;
		float DeltaV2 = uv2.y - uv0.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		glm::vec3 tangent, bitangent;

		tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
		bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
		bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

		if (isnanv(tangent) || isinfv(tangent)) {
			static int qwe = 0;
			qwe++;
			continue;
		}
		if (isnanv(bitangent) || isinfv(bitangent)) {
			static int qwe = 0;
			qwe++;
			continue;
		}

		// Set the same tangent for all three vertices of the triangle.
		// They will be merged later, in vboindexer.cpp
		this->tangents[i0] += tangent;
		this->tangents[i1] += tangent;
		this->tangents[i2] += tangent;

		// Same thing for binormals
		this->bitangents[i0] += bitangent;
		this->bitangents[i1] += bitangent;
		this->bitangents[i2] += bitangent;
	}

	// See "Going Further"
#if 1
	for (int i = 0; i < this->numVertices; i += 1) {
		glm::vec3 & n = this->normals[i];
		glm::vec3 & t = this->tangents[i];
		glm::vec3 & b = this->bitangents[i];

		if (isnanv(n) || isnanv(t) || isnanv(b)) {
			continue;
		}
		if (isinfv(n) || isinfv(t) || isinfv(b)) {
			continue;
		}

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));
		b = glm::cross(t, n);

		// Calculate handedness
#if 1
		if (glm::dot(glm::cross(n, t), b) < 0.0f){
			t = t * -1.0f;
		}
#endif
		b = glm::cross(t, n);
	}
#endif



#if 0
	for (unsigned int face = 0; face < (this->indices.size() / 3); ++face) {
		unsigned int i0 = this->indices[face + 0];
		unsigned int i1 = this->indices[face + 1];
		unsigned int i2 = this->indices[face + 2];

		// Do not process degenerate triangles:
		if (i0 == i1 || i0 == i2 || i1 == i2) {
			continue;
		}

		glm::vec3& v0 = this->vertices[i0];
		glm::vec3& v1 = this->vertices[i1];
		glm::vec3& v2 = this->vertices[i2];

		glm::vec2 uv0 = this->textureCoords[i0];
		glm::vec2 uv1 = this->textureCoords[i1];
		glm::vec2 uv2 = this->textureCoords[i2];

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		if (std::isinf(r)) {
			continue;
		}
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

		// Add computed tangent and bitangent to the running sum at each vertex (they were zeroed to begin with)
		this->tangents[i0] += tangent;
		this->tangents[i1] += tangent;
		this->tangents[i2] += tangent;

		this->bitangents[i0] += bitangent;
		this->bitangents[i1] += bitangent;
		this->bitangents[i2] += bitangent;

		if (isnanv(this->tangents[i0])) {
			static int qwe = 0;
			qwe++;
		}
		if (isnanv(this->tangents[i1])) {
			static int qwe = 0;
			qwe++;
		}
		if (isnanv(this->tangents[i2])) {
			static int qwe = 0;
			qwe++;
		}
	}

	for (int i = 0; i < this->numVertices; ++i) {
		if (this->tangents[i] != glm::vec3(0.0f, 0.0f, 0.0f)) {
			this->tangents[i]   = glm::normalize(this->tangents[i]);
		}
		if (this->bitangents[i] != glm::vec3(0.0f, 0.0f, 0.0f)) {
			this->bitangents[i] = glm::normalize(this->bitangents[i]);
		}
	}
#endif
}

void SimpleMesh::makeVBOs() {

	this->computeTangentsAndBitangents();

    if (this->vertices != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPositions);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->vertices, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("\nERROR: Uninited vertices");
        return;
    }

    if (this->normals != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboNormals);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboNormals);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->normals, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("\nERROR: Uninited normals");
        return;
    }

    if (this->tangents != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboTangents);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTangents);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->tangents, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("\nERROR: Uninited tangents");
    }

    if (this->bitangents != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboBitangents);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBitangents);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->bitangents, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("\nERROR: Uninited bitangents");
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
        FRAMEWORK->GetLogger()->errlog("\nERROR: Uninited indices");
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

    	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_vTangent)) {
    		GLint tangentHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vTangent);
    		GLCALL(glEnableVertexAttribArray, tangentHandle);
    		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTangents);
    		GLCALL(glVertexAttribPointer, tangentHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    	}

    	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_vBitangent)) {
    		GLint bitangentHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vBitangent);
    		GLCALL(glEnableVertexAttribArray, bitangentHandle);
    		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboBitangents);
    		GLCALL(glVertexAttribPointer, bitangentHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    	}
    }
    //
    // TODO [Hack] Do this better, maybe:
    // No materials in depth pass:
    if (!currentShaderSet->IsDepthPass()) {
    	this->materialRef->WriteMaterialToShader();
    	if (this->materialRef->HasTexture()) {
    		this->materialRef->GetTextureAsset()->Draw(0);

			VERIFY(currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in), "Shader has a texture");
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

    	if (this->materialRef->HasNormalMap()) {
    		this->materialRef->GetNormalMapTextureAsset()->Draw(3);
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
    if (this->tangents != nullptr) {
        delete this->tangents;
    }
    if (this->bitangents != nullptr) {
        delete this->bitangents;
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
    if (this->vboTangents != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboTangents);
    }
    if (this->vboBitangents != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboBitangents);
    }
    if (this->vboTextureCoords != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboTextureCoords);
    }
    if (this->vboIndices != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboIndices);
    }
}

