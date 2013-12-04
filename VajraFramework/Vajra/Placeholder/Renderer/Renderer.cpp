#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/Input/Input.h"
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


bool setupGraphics(int w, int h) {
    gWidth = w;
    gHeight = h;


    FRAMEWORK->GetLogger()->dbglog("setupGraphics(%d, %d)", w, h);

    // Make sure OpenGLWrapper is init'd:
    FRAMEWORK->GetOpenGLWrapper()->PrintGLVersion();

    ENGINE->GetSceneGraph()->Initialize();

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    glEnable(GL_TEXTURE_2D);
    checkGlError("glEnable(GL_TEXTURE_2D)");
    //
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");

#if PLATFORM_DESKTOP
    // TODO [Implement] Move this to shader specific code so that we can have only some shaders draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    
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

	GLint mvpMatrixHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_mvpMatrix);
    glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    return;
}


void renderFrame(float dt) {
    setupMatrixes(dt);

    static float grey;
    grey += dt / 5.0f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(0.5f, grey, grey, 1.0f);                    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    checkGlError("glClear");

    float deltaTime = ENGINE->GetTimer()->GetDeltaFrameTime();
    {
        // Temp, testing transforms:
        GameObject* quad = ENGINE->GetSceneGraph()->GetGameObjectById(107);
        if (quad != nullptr) {
        	Transform* transform = quad->GetTransform();
        	if (transform != nullptr) {
        		// transform->SetOrientation(90.0f, YAXIS);
        		transform->Translate(0.5f * deltaTime, transform->GetForward());
        		transform->Rotate(60.0f * deltaTime, YAXIS);
        		transform->Rotate(30.0f * deltaTime, -transform->GetLeft());
        	}
        }
    }
    {
        // Temp, testing transforms:
        GameObject* quad = ENGINE->GetSceneGraph()->GetGameObjectById(108);
        if (quad != nullptr) {
        	Transform* transform = quad->GetTransform();
        	if (transform != nullptr) {
        		// transform->SetOrientation(90.0f, YAXIS);
        		// transform->Translate(0.04f, transform->GetForward());
        		// transform->Rotate(30.0f * deltaTime, YAXIS);
        		// transform->Rotate(0.4f, -transform->GetLeft());
        	}
        }
    }
    {
        // Temp, testing transforms:
        GameObject* quad = ENGINE->GetSceneGraph()->GetGameObjectById(109);
        if (quad != nullptr) {
        	Transform* transform = quad->GetTransform();
        	if (transform != nullptr) {
        		// transform->SetOrientation(90.0f, YAXIS);
        		// transform->Translate(0.04f, transform->GetForward());
        		// transform->Rotate(50.0f * deltaTime, YAXIS);
        		// transform->Rotate(0.4f, -transform->GetLeft());
        	}
        }
    }
    {
        // Temp, testing transforms:
        GameObject* dlight = (GameObject*)ENGINE->GetSceneGraph()->GetMainDirectionalLight()->GetObject();
        if (dlight != nullptr) {
        	Transform* transform = dlight->GetTransform();
        	if (transform != nullptr) {
        		// transform->SetOrientation(90.0f, YAXIS);
        		transform->Translate(0.1f * deltaTime, transform->GetForward());
        		transform->Rotate(-50.0f * deltaTime, YAXIS);
        		// transform->Rotate(100.0f * deltaTime, -transform->GetLeft());
        	}
        }
    }
    {
        // Sending arbit message for testing
        {
			const Message* const message = new Message();
			ENGINE->GetMessageHub()->SendPointcastMessage(message, 106);
			delete message;
        }
    }
    {
    	GameObject* wavybox = ENGINE->GetSceneGraph()->GetGameObjectById(109);
    	// Transform* transform = wavybox->GetTransform();
    	// transform->Rotate(10.0f * deltaTime, YAXIS);
    	// transform->Translate(0.05f * deltaTime, transform->GetForward());
    	Armature* armature = wavybox->GetComponent<Armature>();
#if 1
    	{
			Bone* someBone = armature->GetBoneByName("LadyAssassinWeights_BackJoint01");
			someBone->Rotate(5.0f * deltaTime, XAXIS, false);
    	}
#endif
#if 1
    	{
			Bone* someBone = armature->GetBoneByName("LadyAssassinWeights_Left_ElbowJoint01");
			someBone->Rotate(-5.0f * deltaTime, YAXIS, false);
    	}
#endif
#if 1
    	{
			Bone* someBone = armature->GetBoneByName("LadyAssassinWeights_Left_ShoulderJoint01");
			someBone->Rotate(-5.0f * deltaTime, YAXIS, true);
    	}
#endif
#if 0
    	{
			Bone* someBone = armature->GetBoneByName("LadyAssassinWeights_NeckJoint01");
			someBone->Translate(0.5f * deltaTime, YAXIS, false);
    	}
#endif
#if 0
    	{
			Bone* someBone = armature->GetBoneByName("joint13");
			someBone->Rotate(5.0f * deltaTime, ZAXIS, false);
    	}
#endif
#if 0
    	{
			Bone* someBone = armature->GetBoneByName("joint6");
			someBone->Rotate(5.0f * deltaTime, ZAXIS, true);
    	}
#endif
    }
#if 1
    {
        if(ENGINE->GetInput()->GetTouchCount() > 0)
        {
            Touch temp = ENGINE->GetInput()->GetTouch(0);
            float touchDisplacement = temp.pos.x - temp.prevPos.x;
            glm::vec2 moveDir = temp.pos - temp.prevPos;
            GameObject* camera = (GameObject*)ENGINE->GetSceneGraph()->GetMainCamera()->GetObject();
            Transform* transform = camera->GetTransform();
            float camMag = glm::distance(transform->GetPosition(), ZERO_VEC3);
            transform->SetPosition(0.0f, 0.0f, 0.0f);
            transform->Rotate(-1 * moveDir.x, transform->GetUp());
            transform->Rotate(moveDir.y, transform->GetLeft());
            transform->Translate(-camMag, transform->GetForward());
        }
    }
#endif
    ENGINE->DoFrame();

    //printFrameTimeStats();

    modelMatrix = glm::rotate(0.0f, 0.0f, 1.0f, 0.0f);
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    // FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->SetMVPMatrixHandle(mvpMatrix);


    return;
}
