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
	
	void CreateShaderSet(std::string shaderName, \
		std::string vshaderName, std::string fshaderName);
	//
	void SetCurrentShaderSet(std::string shaderName);
	inline ShaderSet* GetCurrentShaderSet() { return this->currentShaderSet; }

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
