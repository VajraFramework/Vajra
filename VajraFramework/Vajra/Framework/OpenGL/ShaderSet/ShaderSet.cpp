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
#include <vector>

// Forward Declarations:
GLuint createProgram(std::string vShaderName, std::string fShaderName);	// REMOVE THIS


ShaderSet::ShaderSet(std::string inVshaderName, std::string inFshaderName) {
	this->init(inVshaderName, inFshaderName);
}

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
    VERIFY(this->shaderProgram != 0, "Successfully created shader program");
}

void ShaderSet::init(std::string inVshaderName, std::string inFshaderName) {
    FRAMEWORK->GetLogger()->dbglog("\nCreating ShaderProgram with %s, %s", \
                                inVshaderName.c_str(), inFshaderName.c_str());
    this->shaderProgram = createProgram(inVshaderName, inFshaderName);
    if (!this->shaderProgram) {
        FRAMEWORK->GetLogger()->errlog("Could not create program.");
        exit(0);
        // TODO [Cleanup] ASSERT_HERE
    }
    FRAMEWORK->GetLogger()->dbglog("\nShaderProgram: %d\n", this->shaderProgram);

    this->shaderHandles = new ShaderHandles(this->shaderProgram);

    // Get Attribute Handles from Shaders:
    //
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_attribute, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_vPosition);
    //
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_attribute, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_vNormal);
    //
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_attribute, SHADER_VARIABLE_DATATYPE_vec2,
    									 SHADER_VARIABLE_VARIABLENAME_uvCoords_in);

    // Get Uniform Handles from Shaders:
    //
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_mat4,
    									 SHADER_VARIABLE_VARIABLENAME_mvpMatrix);
    //
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_mat4,
    									 SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix);


    // Lights and Materials:

    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_DLight0Direction);
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor);
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor);
    this->shaderHandles->AddShaderHandle(SHADER_VARIABLE_QUALIFIER_uniform, SHADER_VARIABLE_DATATYPE_vec4,
    									 SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor);
}

void ShaderSet::destroy() {
	// TODO [Implement] Figure out if there is anything to clean up here
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

char* readShaderFromFile(std::string shaderName) {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + shaderName;

    std::vector<char> v;
    if (FILE *fp = fopen(path.c_str(), "r")) {
        char buf[1024];
        while (size_t len = fread(buf, 1, sizeof(buf), fp)) {
            v.insert(v.end(), buf, buf + len);
        }
        fclose(fp);
    }

    FRAMEWORK->GetLogger()->dbglog("\nRead in %s Shader length: %d", shaderName.c_str(), v.size());

    char* shader = (char *)malloc(v.size() + 1);
    for (unsigned int i = 0; i < v.size(); ++i) {
        shader[i] = v[i];
    }
    shader[v.size()] = '\0';

    FRAMEWORK->GetLogger()->dbglog("\nSize of %s Shader buffer: %d", shaderName.c_str(), strlen(shader));

    FRAMEWORK->GetLogger()->dbglog("\nShader:\n%s", shader);
    FRAMEWORK->GetLogger()->dbglog("\nEnd Shader");

    return shader;
}

void ShaderSet::createShaderProgram() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->shaderSpecificationName;

    std::ifstream shaderSpecFile(path.c_str());
    VERIFY_LOG(shaderSpecFile.good(), "Successfully opened shader spec file %s for reading", path.c_str());

    shaderSpecFile >> this->vshaderSrcName;
    shaderSpecFile >> this->fshaderSrcName;

    FRAMEWORK->GetLogger()->dbglog("\nVshader src name: %s", this->vshaderSrcName.c_str());
    FRAMEWORK->GetLogger()->dbglog("\nFshader src name: %s", this->fshaderSrcName.c_str());

    this->createVShader();
    this->createFShader();
}

#define VARIABLES_STRING "__VARIABLES__"
#define VARIABLES_END_STRING "__VARIABLES_END__"
//
#define SOURCE_STRING "__SOURCE__"
#define SOURCE_END_STRING "__SOURCE_END__"

void ShaderSet::createVShader() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->vshaderSrcName;

    std::ifstream vshadersrcFile(path.c_str());
    VERIFY_LOG(vshadersrcFile.good(), "Successfully opened vertex shader src file %s for reading", path.c_str());

    // TODO [Implement] Move this parsing somewhere else

    std::string vshaderSourceBuffer;

    ReadTextFileTillStringToken(vshadersrcFile, VARIABLES_STRING);
    {
		std::string buffer;
		do {
			vshadersrcFile >> buffer;
			if (buffer != VARIABLES_END_STRING) {
				this->variablesUsed.push_back(buffer);
				vshaderSourceBuffer += ShaderSetCreationHelper::GetVariableDeclarationForVariableName(buffer) + "\n";
			}
		} while (buffer != VARIABLES_END_STRING);
    }

    ReadTextFileTillStringToken(vshadersrcFile, SOURCE_STRING);
    {
		std::string buffer;
		do {
			std::getline(vshadersrcFile, buffer);
			if (buffer != SOURCE_END_STRING) {
				vshaderSourceBuffer += buffer + "\n";
			}
		} while (buffer != SOURCE_END_STRING);
    }

    FRAMEWORK->GetLogger()->dbglog("\nDone reading vertex shader source:\n%s", vshaderSourceBuffer.c_str());
}

void ShaderSet::createFShader() {
    std::string path = FRAMEWORK->GetFileSystemUtils()->GetDeviceShaderResourcesPath() + this->fshaderSrcName;

    std::ifstream fshadersrcFile(path.c_str());
    VERIFY_LOG(fshadersrcFile.good(), "Successfully opened fragment shader src file %s for reading", path.c_str());
}

GLuint createProgram(std::string vShaderName, std::string fShaderName) {
    char* vShaderSource = readShaderFromFile(vShaderName);
    char* fShaderSource = readShaderFromFile(fShaderName);

    FRAMEWORK->GetLogger()->dbglog("\nCompiling Shaders");

    printGLString("OpenGL Version", GL_VERSION);
    printGLString("Shader Language Version", GL_SHADING_LANGUAGE_VERSION);

    FRAMEWORK->GetLogger()->dbglog("\nCompiling Vertex Shader");
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vShaderSource);
    if (!vertexShader) {
        return 0;
    }

    FRAMEWORK->GetLogger()->dbglog("\nCompiling Fragment Shader");
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, fShaderSource);
    if (!pixelShader) {
        return 0;
    }

    FRAMEWORK->GetLogger()->dbglog("\nLinking Shaders");

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            FRAMEWORK->GetLogger()->dbglog("\nLink error info_log bufLength: %d", bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    FRAMEWORK->GetLogger()->errlog("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;

            return 0;
        }

    }
    return program;
}
