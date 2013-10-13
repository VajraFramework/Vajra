#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/DeviceUtils/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

// TODO [Cleanup] This here is probably not needed
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_DESKTOP)
#include "png.h"
#endif // PLATFORM_ANDROID || PLATFORM_DESKTOP

#include <fstream>
#include <string>
#include <vector>

static int gWidth, gHeight;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 mvpMatrix;
//
GLint textureHandle = 0;


// Stuff to initialize Mesh with:
std::vector<glm::vec3> meshPositions;
std::vector<glm::vec3> meshNormals;
std::vector<glm::vec2> meshTextureCoords;
std::vector<unsigned int> meshIndices;
Mesh *mesh;
//

bool setupGraphics(int w, int h) {
    gWidth = w;
    gHeight = h;


    FRAMEWORK->GetLogger()->dbglog("setupGraphics(%d, %d)", w, h);

    // Make sure OpenGLWrapper is init'd:
    FRAMEWORK->GetOpenGLWrapper()->PrintGLVersion();

    ENGINE->GetSceneGraph()->Initialize();

    glViewport(0, 0, w, h);
    checkGlError("glViewport");


    // Load image
    std::string imagePath = FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesPath() + "square.png";
    textureHandle = loadGLTextureFromPNG(imagePath.c_str());

    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");

    
    return true;
}

glm::vec3 eyePosition(2.0f, 2.0f, 2.0f);
glm::vec3 lookAtPosition(0.0f, 0.0f, 0.0f);
glm::vec3 upVector(0.0f, 1.0f, 0.0f);

void setupMatrixes(float dt) {
    static float angle = 0.0f;
    angle += dt * 180.0f / 2.0f;

    modelMatrix = glm::translate(0.0f, 0.0f, 0.5f) * glm::rotate(angle, 0.0f, 1.0f, 0.0f);
    // modelMatrix = glm::rotate(angle, 0.0f, 1.0f, 0.0f) * glm::translate(0.0f, 0.0f, 0.5f);

    viewMatrix = glm::lookAt(eyePosition, lookAtPosition, upVector);

    projectionMatrix = glm::perspective(60.0f, (float)gWidth / (float)gHeight, 0.1f, 8000.0f);

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->SetMVPMatrixHandle(mvpMatrix);

    return;
}


void renderFrame(float dt) {
    setupMatrixes(dt);

    static float grey;
    grey += dt / 5.0f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(1.0, grey, grey, 1.0f);                    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    checkGlError("glClear");

    {
        // Temp, testing transforms:
        GameObject* quad = ENGINE->GetSceneGraph()->GetGameObjectById(104);
        if (quad != nullptr) {
        	Transform* transform = quad->GetTransform();
        	if (transform != nullptr) {
        		// transform->SetOrientation(90.0f, YAXIS);
        		transform->Translate(0.01f, transform->GetForward());
        		transform->Rotate(0.8f, YAXIS);
        		transform->Rotate(0.4f, -transform->GetLeft());
        	}
        }
        Message* message = new Message();
        ENGINE->GetMessageHub()->SendPointcastMessage(message, 104);
    }

    ENGINE->UpdateScene();
    ENGINE->RenderScene();

    modelMatrix = glm::rotate(0.0f, 0.0f, 1.0f, 0.0f);
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    // FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->SetMVPMatrixHandle(mvpMatrix);

    ENGINE->RenderScene();


    return;
}
