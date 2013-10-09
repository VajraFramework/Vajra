#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

#define DEFAULT_SHADER_NAME "smplshdr"
#define DEFAULT_VSHADER "SimpleVertexShader.vertexshader"
#define DEFAULT_FSHADER "SimpleFragmentShader.fragmentshader"

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

void OpenGLWrapper::CreateShaderSet(std::string shaderName, \
        std::string vshaderName, std::string fshaderName) {

    ShaderSet* shaderSet = new ShaderSet(vshaderName, fshaderName);
    // TODO [Cleanup] : Check for duplicates:
    this->shaderSets[shaderName] = shaderSet;

    if (this->currentShaderSet == 0) {
        this->currentShaderSet = shaderSet;
    }
}

void OpenGLWrapper::SetCurrentShaderSet(std::string shaderName) {
    // TODO [Cleanup] : Check if it actually exists:
    this->currentShaderSet = this->shaderSets[shaderName];
}

void OpenGLWrapper::init() {
    FRAMEWORK->GetLogger()->dbglog("In OpenGLWrapper::init()\n");

    glEnable(GL_DEPTH_TEST);

    this->currentShaderSet = nullptr;
    this->CreateShaderSet(DEFAULT_SHADER_NAME, \
                              DEFAULT_VSHADER, DEFAULT_FSHADER);

}

void OpenGLWrapper::destroy() {
}
