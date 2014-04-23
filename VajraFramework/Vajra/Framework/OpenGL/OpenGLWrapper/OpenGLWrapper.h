#ifndef OPENGLWRAPPER_H
#define OPENGLWRAPPER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#include <map>
#include <string>

// Forward Declarations:
class OpenGLCounter;
class ShaderSet;

class OpenGLWrapper : public Object {
public:
	~OpenGLWrapper();
	
	void PrintGLVersion();
	
	// !! Must be called on the gl thread:
	void CreateShaderSet(std::string shaderName, std::string shaderSpecificationName, bool hasTransperancy, bool isOverlay, bool isDepthPass);
	//
	void SetCurrentShaderSet(std::string shaderName);
	inline ShaderSet* GetCurrentShaderSet() { return this->currentShaderSet; }

	ShaderSet* GetShaderSetByName(std::string shaderName);

	void GetAllAvailableShaderNames(std::vector<std::string>& out_shaderNames);

	OpenGLCounter* GetOpenGLCounter() { return this->glCounter; }

	inline void FreeGLBuffer (GLuint* buffer );
	inline void FreeGLTexture(GLuint* texture);

	// !! Must be called on the gl thread:
	void FreeUnusedGLResources();

private:
	// !! Must be called on the gl thread:
	OpenGLWrapper();
	void init();
	void destroy();

	// !! Must be called on the gl thread:
	void freeUnusedGLBuffers();
	// !! Must be called on the gl thread:
	void freeUnusedGLTextures();

	std::map<std::string /* shaderName */, ShaderSet*> shaderSets;
	ShaderSet *currentShaderSet;

	GLuint positionHandle;
	GLuint textureCoordsHandle;

	OpenGLCounter* glCounter;

	std::vector<GLuint> glBuffersToBeFreed;
	std::vector<GLuint> glTexturesToBeFreed;

	friend class Framework;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

void OpenGLWrapper::FreeGLBuffer(GLuint* buffer) {
	// TODO [Hack] For some reason the simulator crashes in glDrawElements() when we do this
#ifdef __APPLE__
#if TARGET_IPHONE_SIMULATOR
	return;
#endif
#endif
	this->glBuffersToBeFreed.push_back(*buffer);
	*buffer = 0;
}

void OpenGLWrapper::FreeGLTexture(GLuint* texture) {
	// TODO [Hack] For some reason the simulator crashes in glDrawElements() when we do this
#ifdef __APPLE__
#if TARGET_IPHONE_SIMULATOR
	return;
#endif
#endif
	this->glTexturesToBeFreed.push_back(*texture);
	*texture = 0;
}

#endif // OPENGLWRAPPER_H
