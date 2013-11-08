#include "Vajra/Framework/OpenGL/ShaderSet/DLightShaderHandles.h"
#include "Vajra/Utilities/Utilities.h"

DLightShaderHandles::DLightShaderHandles(GLuint shaderProgram) {
	this->init(shaderProgram);
}

DLightShaderHandles::~DLightShaderHandles() {
	this->destroy();
}

void DLightShaderHandles::init(GLuint shaderProgram) {
    this->directionHandle = glGetUniformLocation(shaderProgram, "DLight0Direction");
    checkGlError("glGetUniformLocation");
    ASSERT_LOG(this->directionHandle != -1, "Got uniform location from shader for DLight0Direction, %d", this->directionHandle);
    //
    this->ambientColorHandle = glGetUniformLocation(shaderProgram, "DLight0AmbientColor");
    checkGlError("glGetUniformLocation");
    ASSERT_LOG(this->ambientColorHandle != -1, "Got uniform location from shader for DLight0AmbientColor, %d", this->ambientColorHandle);
    //
    this->diffuseColorHandle = glGetUniformLocation(shaderProgram, "DLight0DiffuseColor");
    checkGlError("glGetUniformLocation");
    ASSERT_LOG(this->diffuseColorHandle != -1, "Got uniform location from shader for DLight0DiffuseColor, %d", this->diffuseColorHandle);
    //
    this->specularColorHandle = glGetUniformLocation(shaderProgram, "DLight0SpecularColor");
    checkGlError("glGetUniformLocation");
    ASSERT_LOG(this->specularColorHandle != -1, "Got uniform location from shader for DLight0SpecularColor, %d", this->specularColorHandle);
}

void DLightShaderHandles::destroy() {
}
