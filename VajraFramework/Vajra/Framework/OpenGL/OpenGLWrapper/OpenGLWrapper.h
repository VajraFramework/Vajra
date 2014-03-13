#ifndef OPENGLWRAPPER_H
#define OPENGLWRAPPER_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include <map>
#include <string>

// Forward Declarations:
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

private:
	OpenGLWrapper();
	void init();
	void destroy();

	std::map<std::string /* shaderName */, ShaderSet*> shaderSets;
	ShaderSet *currentShaderSet;

	GLuint positionHandle;
	GLuint textureCoordsHandle;

	friend class Framework;
};

#endif // OPENGLWRAPPER_H
