#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include <Vajra/Framework/OpenGL/ShaderSet/ShaderSetCreationHelper/ShaderSetCreationHelper.h>
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Forward Declarations:
GLuint loadShader(GLenum shaderType, const char* pSource);

ShaderSet::ShaderSet(std::string inShaderSpecificationName) {
	this->init(inShaderSpecificationName);
}

ShaderSet::~ShaderSet() {
	this->destroy();
}

void ShaderSet::init(std::string inShaderSpecificationName) {
    FRAMEWORK->GetLogger()->dbglog("\nCreating ShaderProgram shader specification file %s", \
                                inShaderSpecificationName.c_str());
    this->shaderSpecificationName = inShaderSpecificationName;

    this->createShaderProgram();

    VERIFY_LOG(this->shaderProgram != 0, "Successfully created shader program: %u", this->shaderProgram);

    this->createHandles();
}

void ShaderSet::destroy() {
	// TODO [Implement] Figure out if there is anything to clean up here
}

void ShaderSet::createShaderProgram() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->shaderSpecificationName;

    std::ifstream shaderSpecFile(path.c_str());
    VERIFY_LOG(shaderSpecFile.good(), "Successfully opened shader spec file %s for reading", path.c_str());

    shaderSpecFile >> this->vshaderSrcName;
    shaderSpecFile >> this->fshaderSrcName;

    FRAMEWORK->GetLogger()->dbglog("\nVshader src name: %s", this->vshaderSrcName.c_str());
    FRAMEWORK->GetLogger()->dbglog("\nFshader src name: %s", this->fshaderSrcName.c_str());

    printGLString("OpenGL Version", GL_VERSION);
    printGLString("Shader Language Version", GL_SHADING_LANGUAGE_VERSION);

    this->createVShader();
    this->createFShader();

	FRAMEWORK->GetLogger()->dbglog("\nLinking Shaders");

    this->shaderProgram = glCreateProgram();
    if (this->shaderProgram) {
        glAttachShader(this->shaderProgram, this->compiledVShader);
        checkGlError("glAttachShader");
        glAttachShader(this->shaderProgram, this->compiledFShader);
        checkGlError("glAttachShader");
        glLinkProgram(this->shaderProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, &bufLength);
            FRAMEWORK->GetLogger()->dbglog("\nLink error info_log bufLength: %d", bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(this->shaderProgram, bufLength, NULL, buf);
                    FRAMEWORK->GetLogger()->errlog("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(this->shaderProgram);
            this->shaderProgram = 0;

            VERIFY(0, "Shader linking failed");
        }
    }
}

#define VARIABLES_STRING "__VARIABLES__"
#define VARIABLES_END_STRING "__VARIABLES_END__"
//
#define SOURCE_STRING "__SOURCE__"
#define SOURCE_END_STRING "__SOURCE_END__"

void ShaderSet::createVShader() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->vshaderSrcName;

	std::string buffer;
    {
    	std::ifstream vshadersrcFile(path.c_str());
    	VERIFY_LOG(vshadersrcFile.good(), "Successfully opened vertex shader src file %s for reading", path.c_str());
    	while (vshadersrcFile.good()) {
    		std::string line;
    		std::getline(vshadersrcFile, line);
    		buffer += line + "\n";
    	}
    	buffer = ShaderSetCreationHelper::CleanupShaderSourceForPreprocessorDirectives(buffer);
    }
    std::istringstream vshader_cleanedupSource(buffer);


    // TODO [Implement] Move this parsing somewhere else

    std::string vshaderSourceBuffer;

    ReadTextFileTillStringToken(vshader_cleanedupSource, VARIABLES_STRING);
    {
		do {
			std::getline(vshader_cleanedupSource, buffer);
			if (buffer != "" && buffer != VARIABLES_END_STRING) {
				this->variablesUsed.push_back(buffer);
				vshaderSourceBuffer += ShaderSetCreationHelper::GetVariableDeclarationForVariableName(buffer) + "\n";
			}
		} while (buffer != VARIABLES_END_STRING && vshader_cleanedupSource.good());
    }

    ReadTextFileTillStringToken(vshader_cleanedupSource, SOURCE_STRING);
    {
		std::string buffer;
		do {
			std::getline(vshader_cleanedupSource, buffer);
			if (buffer != SOURCE_END_STRING) {
				vshaderSourceBuffer += buffer + "\n";
			}
		} while (buffer != SOURCE_END_STRING && vshader_cleanedupSource.good());
    }

    vshaderSourceBuffer = ShaderSetCreationHelper::CleanupShaderSourceForPreprocessorDirectives(vshaderSourceBuffer);

    FRAMEWORK->GetLogger()->dbglog("\nDone reading vertex shader source:\n%s", vshaderSourceBuffer.c_str());

    FRAMEWORK->GetLogger()->dbglog("\nCompiling Vertex Shader");
    this->compiledVShader = loadShader(GL_VERTEX_SHADER, vshaderSourceBuffer.c_str());
    VERIFY_LOG(this->compiledVShader != 0, "\nVertex shader compiled successfully");
}

void ShaderSet::createFShader() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->fshaderSrcName;

    std::ifstream fshadersrcFile(path.c_str());
    VERIFY_LOG(fshadersrcFile.good(), "Successfully opened fragment shader src file %s for reading", path.c_str());

    std::string fshaderSourceBuffer;

    // TODO [Implement] Move this parsing somewhere else
	{
		std::string buffer;
		do {
			std::getline(fshadersrcFile, buffer);
			fshaderSourceBuffer += buffer + "\n";
		} while (fshadersrcFile.good());
    }

	fshaderSourceBuffer = ShaderSetCreationHelper::CleanupShaderSourceForPreprocessorDirectives(fshaderSourceBuffer);

    FRAMEWORK->GetLogger()->dbglog("\nDone reading fragment shader source:\n%s", fshaderSourceBuffer.c_str());

    FRAMEWORK->GetLogger()->dbglog("\nCompiling Fragment Shader");
    this->compiledFShader = loadShader(GL_FRAGMENT_SHADER, fshaderSourceBuffer.c_str());
    VERIFY_LOG(this->compiledFShader != 0, "\nFragment shader compiled successfully");
}

void ShaderSet::createHandles() {
	this->shaderHandles = new ShaderHandles(this->shaderProgram);

	for (std::string shaderVariableName : this->variablesUsed) {
		std::string qualifierString = ShaderSetCreationHelper::GetVariableQualifierForVariableName(shaderVariableName);
		std::string datatypeString  = ShaderSetCreationHelper::GetVariableDatatypeForVariableName(shaderVariableName);
		Shader_variable_qualifier_t qualifier = GetShaderVariableQualifierFromString(qualifierString);
		Shader_variable_datatype_t datatype   = GetShaderVariableDatatypeFromString(datatypeString);
		Shader_variable_variablename_id_t variablename_id = GetShaderVariableVariableNameIdFromString(shaderVariableName);

		this->shaderHandles->AddShaderHandle(qualifier, datatype, variablename_id);
	}
}


////////////////////////////////////////////////////////////////////////////////

// Shader Loading & Compilation Helper Functions:
GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            FRAMEWORK->GetLogger()->dbglog("\nCould not compile shader");
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            FRAMEWORK->GetLogger()->dbglog("\ninfoLen: %d", infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    FRAMEWORK->GetLogger()->dbglog("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}
