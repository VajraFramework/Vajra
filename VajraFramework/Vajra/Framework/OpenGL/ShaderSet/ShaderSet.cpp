#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

// Forward Declarations:
GLuint createProgram(std::string vShaderName, std::string fShaderName);


ShaderSet::ShaderSet(std::string inVshaderName, std::string inFshaderName) {
    FRAMEWORK->GetLogger()->dbglog("\nCreating ShaderProgram with %s, %s", \
                                inVshaderName.c_str(), inFshaderName.c_str());
    this->shaderProgram = createProgram(inVshaderName, inFshaderName);
    if (!this->shaderProgram) {
        FRAMEWORK->GetLogger()->errlog("Could not create program.");
        exit(0);
        // TODO [Cleanup] ASSERT_HERE
    }
    FRAMEWORK->GetLogger()->dbglog("\nShaderProgram: %d\n", this->shaderProgram);

    // Get Attribute Handles from Shaders:
    this->positionHandle = glGetAttribLocation(this->GetShaderProgram(), "vPosition");
    checkGlError("glGetAttribLocation");
    FRAMEWORK->GetLogger()->dbglog("glGetAttribLocation(\"vPosition\") = %d\n", this->positionHandle);
    //
    this->normalHandle = glGetAttribLocation(this->GetShaderProgram(), "vNormal");
    checkGlError("glGetAttribLocation");
    FRAMEWORK->GetLogger()->dbglog("glGetAttribLocation(\"vNormal\") = %d\n", this->normalHandle);
    //
    this->textureCoordsHandle = glGetAttribLocation(this->GetShaderProgram(), "uvCoords_in");
    checkGlError("glGetAttribLocation");
    FRAMEWORK->GetLogger()->dbglog("glGetAttribLocation(\"uvCoords_in\") = %d\n", this->textureCoordsHandle);

    // Get Uniform Handles from Shaders:
    this->mvpMatrixHandle = glGetUniformLocation(this->GetShaderProgram(), "mvpMatrix");
    checkGlError("glGetUniformLocation");
    if (this->mvpMatrixHandle == -1) {
        FRAMEWORK->GetLogger()->errlog("mvpMatrix is not a valid shader variable\n");
    }
    FRAMEWORK->GetLogger()->dbglog("glGetUniformLocation(\"mvpMatrix\") = %d\n", this->mvpMatrixHandle);
    //
    this->modelInverseTransposeMatrixHandle = glGetUniformLocation(this->GetShaderProgram(), "modelInverseTransposeMatrix");
    checkGlError("glGetUniformLocation");
    if (this->modelInverseTransposeMatrixHandle == -1) {
        FRAMEWORK->GetLogger()->errlog("modelInverseTransposeMatrix is not a valid shader variable\n");
    }
    FRAMEWORK->GetLogger()->dbglog("glGetUniformLocation(\"modelInverseTransposeMatrix\") = %d\n", this->modelInverseTransposeMatrixHandle);

    // Lights and Materials:
    this->directionalLight = new DLightShaderHandles(this->GetShaderProgram());
}

ShaderSet::~ShaderSet() {
}

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
