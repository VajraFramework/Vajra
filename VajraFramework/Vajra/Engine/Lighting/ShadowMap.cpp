#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

ShadowMap::ShadowMap() {
	// Don't call init here
}

ShadowMap::~ShadowMap() {
	this->destroy();
}

void ShadowMap::Draw() {
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler)) {
		GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler);
		glUniform1i(textureHandle, 2);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
		checkGlError("glBindTexture");
	}
}

void ShadowMap::init() {
}

void ShadowMap::destroy() {
}
