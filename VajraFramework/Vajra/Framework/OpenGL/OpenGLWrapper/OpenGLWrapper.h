#ifndef OPENGLWRAPPER_H
#define OPENGLWRAPPER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

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

	inline void FreeGLBuffer(GLuint* buffer);

	// !! Must be called on the gl thread:
	void FreeUnusedGLBuffers();

private:
	// !! Must be called on the gl thread:
	OpenGLWrapper();
	void init();
	void destroy();


	std::map<std::string /* shaderName */, ShaderSet*> shaderSets;
	ShaderSet *currentShaderSet;

	GLuint positionHandle;
	GLuint textureCoordsHandle;

	OpenGLCounter* glCounter;

	std::vector<GLuint> glBuffersToBeFreed;

	friend class Framework;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

void OpenGLWrapper::FreeGLBuffer(GLuint* buffer) {
	this->glBuffersToBeFreed.push_back(*buffer);
	*buffer = 0;
}

#endif // OPENGLWRAPPER_H
