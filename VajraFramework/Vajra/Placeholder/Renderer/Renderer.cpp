#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/RenderScene/RenderScene.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Loaders/TextureLoader/TextureLoader.h"
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

bool setupGraphics(int w, int h, int dpi) {

	FRAMEWORK->GetDeviceProperties()->SetWidthPixels(w);
	FRAMEWORK->GetDeviceProperties()->SetHeightPixels(h);
    FRAMEWORK->GetDeviceProperties()->SetDPI(dpi);

    FRAMEWORK->GetLogger()->dbglog("setupGraphics(%d, %d)", w, h);

    // Make sure OpenGLWrapper is init'd:
    FRAMEWORK->GetOpenGLWrapper()->PrintGLVersion();

    // TODO [Hack] Get rid of this, somehow
    ENGINE->GetSceneGraph3D()->Initialize();
    ENGINE->GetSceneGraphUi()->Initialize();

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);

    glEnable(GL_TEXTURE_2D);
    checkGlError("glEnable(GL_TEXTURE_2D)");
    //
    glActiveTexture(GL_TEXTURE0);
    checkGlError("glActiveTexture");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if PLATFORM_DESKTOP
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif


#if PLATFORM_DESKTOP
    // TODO [Implement] Move this to shader specific code so that we can have only some shaders draw in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    RenderScene::SetupStuff();
    
    return true;
}


bool renderFrame() {
	double dt = ENGINE->GetTimer()->GetTimeSinceFrameBegin();
#if !defined(PLATFORM_ANDROID)
	// Android regulates the frame rate on the java side
	if (dt < CONST_FRAME_DURATION) {
		return false;
	}
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    checkGlError("glClear");

    // float deltaTime = ENGINE->GetTimer()->GetDeltaFrameTime();

#if 0
    {
        if(ENGINE->GetInput()->GetTouchCount() > 0)
        {
            Touch temp = ENGINE->GetInput()->GetTouch(0);
            //float touchDisplacement = temp.pos.x - temp.prevPos.x;
            glm::vec2 moveDir = temp.pos - temp.prevPos;
            GameObject* camera = (GameObject*)ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject();
            Transform* transform = camera->GetTransform();
            float camMag = glm::distance(transform->GetPosition(), ZERO_VEC3);
            transform->SetPosition(0.0f, 0.0f, 0.0f);
            transform->Rotate(-1 * moveDir.x inRadians, transform->GetUp());
            transform->Rotate(moveDir.y inRadians, transform->GetLeft());
            transform->Translate(-camMag, transform->GetForward());
        }
    }
#endif

#if 0
	DebugDraw::DrawPoint(glm::vec3(1.0f, 1.0f, 1.0f));

	GameObject* torus = ENGINE->GetSceneGraph3D()->GetGameObjectById(115);
	DebugDraw::DrawArrow(ZERO_VEC3, torus->GetTransform()->GetPosition());

	DebugDraw::DrawCube(torus->GetTransform()->GetPosition(), 1.0f);

#endif

    ENGINE->DoFrame();

    // printFrameTimeStats();


    return true;
}
