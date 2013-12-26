#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

OpenGLWrapper::OpenGLWrapper() {
    this->init();
}

OpenGLWrapper::~OpenGLWrapper() {
    this->destroy();
}

void OpenGLWrapper::PrintGLVersion() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
#ifndef PLATFORM_IOS
    // TODO [Implement] Get GLSL Version on ios
    printGLString("GLSL Version", GL_SHADING_LANGUAGE_VERSION);
#endif // PLATFORM_IOS
}

void OpenGLWrapper::CreateShaderSet(std::string shaderName, std::string shaderSpecificationName) {

    ShaderSet* shaderSet = new ShaderSet(shaderSpecificationName);

    VERIFY(this->shaderSets.find(shaderName) == this->shaderSets.end(), "Not duplicate shader set");
    this->shaderSets[shaderName] = shaderSet;

    if (this->currentShaderSet == nullptr) {
        this->currentShaderSet = shaderSet;
    }
}

void OpenGLWrapper::SetCurrentShaderSet(std::string shaderName) {
	VERIFY(this->shaderSets.find(shaderName) != this->shaderSets.end(), "Shader set %s has been created", shaderName.c_str());
    this->currentShaderSet = this->shaderSets[shaderName];
    //
    glUseProgram(this->currentShaderSet->GetShaderProgram());
    checkGlError("glUseProgram");
}

void OpenGLWrapper::GetAllAvailableShaderNames(std::vector<std::string>& out_shaderNames) {
	for (auto shaderSet_it = this->shaderSets.begin(); shaderSet_it != this->shaderSets.end(); ++shaderSet_it) {
		out_shaderNames.push_back(shaderSet_it->first);
	}
}

void OpenGLWrapper::init() {
    FRAMEWORK->GetLogger()->dbglog("In OpenGLWrapper::init()\n");

    glEnable(GL_DEPTH_TEST);

    this->currentShaderSet = nullptr;

	this->CreateShaderSet("simshdr", "SimplestShader.shaderspec");
	this->CreateShaderSet("txrshdr", "TextureShader.shaderspec");
	this->CreateShaderSet("clrshdr", "ColorShader.shaderspec");
	this->CreateShaderSet("bncshdr", "NewBoneColorShader.shaderspec");
	this->CreateShaderSet("bntshdr", "NewBoneTextureShader.shaderspec");
	this->CreateShaderSet("sptshdr", "SpriteTextureShader.shaderspec");
	this->CreateShaderSet("spcshdr", "SpriteColorShader.shaderspec");

}

void OpenGLWrapper::destroy() {
}
