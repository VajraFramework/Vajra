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
	
	void CreateShaderSet(std::string shaderName, std::string shaderSpecificationName, bool hasTransperancy, bool isOverlay, bool isDepthPass);
	//
	void SetCurrentShaderSet(std::string shaderName);
	inline ShaderSet* GetCurrentShaderSet() { return this->currentShaderSet; }

	ShaderSet* GetShaderSetByName(std::string shaderName);

	void GetAllAvailableShaderNames(std::vector<std::string>& out_shaderNames);

	OpenGLCounter* GetOpenGLCounter() { return this->glCounter; }

private:
	OpenGLWrapper();
	void init();
	void destroy();

	std::map<std::string /* shaderName */, ShaderSet*> shaderSets;
	ShaderSet *currentShaderSet;

	GLuint positionHandle;
	GLuint textureCoordsHandle;

	OpenGLCounter* glCounter;

	friend class Framework;
};

#endif // OPENGLWRAPPER_H
