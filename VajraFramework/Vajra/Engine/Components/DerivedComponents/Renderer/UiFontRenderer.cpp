#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/UiFontType/UiFontType.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
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

UiFontRenderer::UiFontRenderer() : Renderer() {
	this->init();
}

UiFontRenderer::UiFontRenderer(Object* object_) : Renderer(object_) {
	this->init();
}

UiFontRenderer::~UiFontRenderer() {
	this->destroy();
}


void UiFontRenderer::initTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile, float fontSize_, UiFontAlignment_type fontAlignment_ /* = UI_FONT_ALIGNMENT_left */) {
	this->fontType = ENGINE->GetAssetLibrary()->GetAsset<UiFontType>(pathToFontSpecificationFile);
	this->SetShaderName(this->fontType->GetShaderName());

	this->fontAlignment = fontAlignment_;
	//
	this->fontSize = fontSize_;
	GameObject* gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");
	gameObjectRef->GetTransform()->SetScale(this->fontSize, this->fontSize, this->fontSize);

	this->required_width  = width;
	this->required_height = height;

	this->textToDisplay = text;
	this->makeText();
	this->initVbos();

	Renderer::addToRenderList();
}

void UiFontRenderer::changeText(std::string text) {
	this->textToDisplay = text;
	this->makeText();
	this->initVbos();
}

void UiFontRenderer::initVbos() {
    if (this->vboPositions != 0 && this->vboTextureCoords != 0 && this->vboIndices != 0) {
		GLCALL(glDeleteBuffers, 1, &this->vboPositions);
		GLCALL(glDeleteBuffers, 1, &this->vboTextureCoords);
		GLCALL(glDeleteBuffers, 1, &this->vboIndices);
    }

    if (this->vertices != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboPositions);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboPositions);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->numVertices, this->vertices, GL_STATIC_DRAW);
    } else {
        FRAMEWORK->GetLogger()->errlog("ERROR: Uninited vertices");
        return;
    }

    if (this->textureCoords != nullptr) {
		GLCALL(glGenBuffers, 1, &this->vboTextureCoords);
		GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTextureCoords);
		GLCALL(glBufferData, GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->numVertices, this->textureCoords, GL_STATIC_DRAW);
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

void UiFontRenderer::HandleMessage(MessageChunk /* messageChunk */) {
}

void UiFontRenderer::Draw() {
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

    ASSERT(this->fontType->GetTextureAsset() != nullptr, "FontType has texture initialized");
	this->fontType->GetTextureAsset()->Draw(0);

	GLint textureCoordsHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in);
	GLCALL(glEnableVertexAttribArray, textureCoordsHandle);
	GLCALL(glBindBuffer, GL_ARRAY_BUFFER, this->vboTextureCoords);
	GLCALL(glVertexAttribPointer, textureCoordsHandle, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLCALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->vboIndices);
    GLCALL(glDrawElements, GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void UiFontRenderer::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->numVertices = 0;
	this->vertices = nullptr;
	this->textureCoords = nullptr;
	this->diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->vboPositions = 0;
	this->vboTextureCoords = 0;
	this->vboIndices = 0;

	this->actual_width  = 0.0f;
	this->actual_height = 0.0f;
	//
	this->required_width  = 0.0f;
	this->required_height = 0.0f;
	//
	this->unscaled_width  = 0.0f;
	this->unscaled_height = 0.0f;

	this->fontSize = 1.0f;
	this->fontAlignment = UI_FONT_ALIGNMENT_left;
}

void UiFontRenderer::destroy() {
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

////////////////////////////////////////////////////////////////////////////////

void UiFontRenderer::makeText() {

	this->unscaled_width = 0.0f;

	unsigned int numVerticesNeeded = this->textToDisplay.size() * 4;
	if (numVerticesNeeded > this->numVertices) {
		if (this->vertices != nullptr) {
			delete this->vertices;
		}
		if (this->textureCoords != nullptr) {
			delete this->textureCoords;
		}
		this->vertices      = new glm::vec3[numVerticesNeeded];
		this->textureCoords = new glm::vec2[numVerticesNeeded];
	}
	this->numVertices = numVerticesNeeded;
	this->indices.resize(this->textToDisplay.size() * 6);

	float woffset = 0.0f;

	for (unsigned int i = 0; i < this->textToDisplay.size (); ++i) {
		char c = this->textToDisplay[i];
		int charIdx = (int) c;
		this->unscaled_width += this->makeACharacter(charIdx, i, woffset);
		woffset += this->fontType->GetCharacterWidth(charIdx);
	}

	float justText_width  = this->unscaled_width  * this->fontSize;

	this->actual_width  = std::max(this->required_width , this->unscaled_width  * this->fontSize);

	// Adjust for font alignment:
	if (this->required_width < justText_width) {
		// Nothing can be done.
	} else {
		switch (this->fontAlignment) {

		case UI_FONT_ALIGNMENT_left: {
			// Nothing to be done
		} break;

		case UI_FONT_ALIGNMENT_center: {
			float offset = (float)(this->required_width - justText_width) / this->fontSize / 2.0f;
			for (unsigned int i = 0; i < this->numVertices; ++i) {
				this->vertices[i].x += offset;
			}
		} break;

		case UI_FONT_ALIGNMENT_right: {
			float offset = (this->required_width - justText_width) / this->fontSize;
			for (unsigned int i = 0; i < this->numVertices; ++i) {
				this->vertices[i].x += offset;
			}
		} break;

		default: {
		} break;

		}
	}
}


float UiFontRenderer::makeACharacter(int charIdxInAscii, int letterIdx, float woffset) {

	float actual_charwidth_on_fontsheet = this->fontType->GetCharacterWidth(charIdxInAscii);

	this->vertices[letterIdx * 4 + 0].x = woffset;
	this->vertices[letterIdx * 4 + 1].x = woffset + actual_charwidth_on_fontsheet;
	this->vertices[letterIdx * 4 + 2].x = woffset + actual_charwidth_on_fontsheet;
	this->vertices[letterIdx * 4 + 3].x = woffset;

	this->vertices[letterIdx * 4 + 0].y = 0.0f;
	this->vertices[letterIdx * 4 + 1].y = 0.0f;
	this->vertices[letterIdx * 4 + 2].y = 0.0f - 1.0f;
	this->vertices[letterIdx * 4 + 3].y = 0.0f - 1.0f;

	// TODO [Hack] Use zorder instead of 0.1f
	this->vertices[letterIdx * 4 + 0].z = 0.1f;
	this->vertices[letterIdx * 4 + 1].z = 0.1f;
	this->vertices[letterIdx * 4 + 2].z = 0.1f;
	this->vertices[letterIdx * 4 + 3].z = 0.1f;

	float each_charwidth_on_fontsheet = 1.0f / NUM_FONT_CHARACTER_COLUMNS;
	float each_charheight_on_fontsheet = 1.0f / NUM_FONT_CHARACTER_ROWS;
	float woffest_into_fontsheet = ((charIdxInAscii - this->fontType->GetStartingCharacter()) % NUM_FONT_CHARACTER_COLUMNS) * each_charwidth_on_fontsheet;
	float hoffest_into_fontsheet = ((charIdxInAscii - this->fontType->GetStartingCharacter()) / NUM_FONT_CHARACTER_ROWS) * each_charheight_on_fontsheet;

	this->textureCoords[letterIdx * 4 + 0].x = woffest_into_fontsheet;
	this->textureCoords[letterIdx * 4 + 1].x = woffest_into_fontsheet + each_charwidth_on_fontsheet * actual_charwidth_on_fontsheet;
	this->textureCoords[letterIdx * 4 + 2].x = woffest_into_fontsheet + each_charwidth_on_fontsheet * actual_charwidth_on_fontsheet;
	this->textureCoords[letterIdx * 4 + 3].x = woffest_into_fontsheet;

	this->textureCoords[letterIdx * 4 + 0].y = hoffest_into_fontsheet;
	this->textureCoords[letterIdx * 4 + 1].y = hoffest_into_fontsheet;
	this->textureCoords[letterIdx * 4 + 2].y = hoffest_into_fontsheet + each_charheight_on_fontsheet;
	this->textureCoords[letterIdx * 4 + 3].y = hoffest_into_fontsheet + each_charheight_on_fontsheet;

	this->indices[letterIdx * 6 + 0] = letterIdx * 4 + 0;
	this->indices[letterIdx * 6 + 1] = letterIdx * 4 + 2;
	this->indices[letterIdx * 6 + 2] = letterIdx * 4 + 1;
	this->indices[letterIdx * 6 + 3] = letterIdx * 4 + 0;
	this->indices[letterIdx * 6 + 4] = letterIdx * 4 + 3;
	this->indices[letterIdx * 6 + 5] = letterIdx * 4 + 2;

	return actual_charwidth_on_fontsheet;
}

