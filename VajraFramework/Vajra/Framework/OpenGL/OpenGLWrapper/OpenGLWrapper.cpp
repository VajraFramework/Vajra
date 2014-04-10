#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLCounter/OpenGLCounter.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSetCreationHelper/ShaderSetCreationHelper.h"
#include "Vajra/Utilities/Utilities.h"

OpenGLWrapper::OpenGLWrapper() {
	// Do not call init() here
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

void OpenGLWrapper::CreateShaderSet(std::string shaderName, std::string shaderSpecificationName, bool hasTransperancy, bool isOverlay, bool isDepthPass) {

    ShaderSet* shaderSet = new ShaderSet(shaderSpecificationName, hasTransperancy, isOverlay, isDepthPass);

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
    GLCALL(glUseProgram, this->currentShaderSet->GetShaderProgram());
}

ShaderSet* OpenGLWrapper::GetShaderSetByName(std::string shaderName) {
	auto it = this->shaderSets.find(shaderName);
	VERIFY(it != this->shaderSets.end(), "Shader set %s found", shaderName.c_str());
	return it->second;
}

void OpenGLWrapper::GetAllAvailableShaderNames(std::vector<std::string>& out_shaderNames) {
	// Return these in the loosely sorted order of opaque first, transperant next, ui last:

	// Consider only the shader sets that are completely opaque and are not an overlay:
	for (auto shaderSet_it = this->shaderSets.begin(); shaderSet_it != this->shaderSets.end(); ++shaderSet_it) {
		ShaderSet* shaderSet = shaderSet_it->second;
		if (!shaderSet->HasTransperancy() && !shaderSet->IsOverlay()) {
			out_shaderNames.push_back(shaderSet_it->first);
		}
	}
	// Consider only the shader sets that have transperancy and are not an overlay:
	for (auto shaderSet_it = this->shaderSets.begin(); shaderSet_it != this->shaderSets.end(); ++shaderSet_it) {
		ShaderSet* shaderSet = shaderSet_it->second;
		if (shaderSet->HasTransperancy() && !shaderSet->IsOverlay()) {
			out_shaderNames.push_back(shaderSet_it->first);
		}
	}
	// Consider only the shader sets that are overlays:
	for (auto shaderSet_it = this->shaderSets.begin(); shaderSet_it != this->shaderSets.end(); ++shaderSet_it) {
		ShaderSet* shaderSet = shaderSet_it->second;
		if (shaderSet->IsOverlay()) {
			out_shaderNames.push_back(shaderSet_it->first);
		}
	}

	ASSERT(out_shaderNames.size() == this->shaderSets.size(), "Did not miss any shader names");
}

void OpenGLWrapper::init() {
    FRAMEWORK->GetLogger()->dbglog("In OpenGLWrapper::init()\n");

    this->glCounter =
    		new OpenGLCounter();

    this->currentShaderSet = nullptr;

    ShaderSetCreationHelper::LoadPreprocessorVariables();

	this->CreateShaderSet("simshdr", "SimplestShader.shaderspec", false, false, false);
	this->CreateShaderSet("txrshdr", "TextureShader.shaderspec", false, false, false);
	this->CreateShaderSet("clrshdr", "ColorShader.shaderspec", true, false, false);
	this->CreateShaderSet("bncshdr", "BoneColorShader.shaderspec", false, false, false);
	this->CreateShaderSet("bntshdr", "BoneTextureShader.shaderspec", false, false, false);
	this->CreateShaderSet("sptshdr", "SpriteTextureShader.shaderspec", true, true, false);
	this->CreateShaderSet("spcshdr", "SpriteColorShader.shaderspec", false, false, false);
	this->CreateShaderSet("ustshdr", "UiSpriteTextureShader.shaderspec", true, true, false);
	this->CreateShaderSet("uscshdr", "UiSpriteColorShader.shaderspec", false, false, false);
	this->CreateShaderSet("prtshdr", "ParticleSystem.shaderspec", true, false, false);
	this->CreateShaderSet("wtrshdr", "Water.shaderspec", false, false, false);
	this->CreateShaderSet("fntshdr", "FontShader.shaderspec", true, true, false);
	//
	// Depth pass shaders:
	this->CreateShaderSet("dbtshdr", "DepthBoneTexturePass.shaderspec", false, false, true);
	this->CreateShaderSet("dbcshdr", "DepthBoneColorPass.shaderspec", false, false, true);
	this->CreateShaderSet("dpcshdr", "DepthColorPass.shaderspec", false, false, true);
	this->CreateShaderSet("dptshdr", "DepthTexturePass.shaderspec", false, false, true);
}

void OpenGLWrapper::destroy() {
	delete this->glCounter;
}
