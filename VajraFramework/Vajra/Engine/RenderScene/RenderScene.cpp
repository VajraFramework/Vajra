#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/RenderScene/RenderScene.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

static GLuint g_depth_fbo_id;

#ifdef PLATFORM_IOS
static GLint g_default_fbo;
#define SCREEN_FRAME_BUFFER g_default_fbo
#else
#define SCREEN_FRAME_BUFFER 0
#endif



void RenderScene::SetupStuff() {

    GLCALL(glEnable, GL_DEPTH_TEST);
	
#ifdef PLATFORM_IOS
	GLCALL(glGetIntegerv, GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	// Create a frame buffer object:
	GLCALL(glGenFramebuffers, 1, &g_depth_fbo_id);
	GLCALL(glBindFramebuffer, GL_FRAMEBUFFER, g_depth_fbo_id);
	
	// Create a render buffer
	GLuint renderBuffer;
    GLCALL(glGenRenderbuffers, 1, &renderBuffer);
    GLCALL(glBindRenderbuffer, GL_RENDERBUFFER, renderBuffer);

    unsigned int depthMap_width, depthMap_height;
    ENGINE->GetShadowMap()->GetDepthMapResolution(depthMap_width, depthMap_height);

	// Create a depth texture:
    GLCALL(glGenTextures, 1, &ENGINE->GetShadowMap()->depthTexture);
	GLCALL(glActiveTexture, GL_TEXTURE2);
    GLCALL(glBindTexture, GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture);
    GLCALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA, depthMap_width, depthMap_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //
    GLCALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLCALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLCALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    GLCALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	GLCALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture, 0);
	
	// Allocate 16-bit depth buffer
	GLCALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, depthMap_width, depthMap_height);
	
	// Attach the render buffer as depth buffer - will be ignored
	GLCALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {	// GLCALL
		ASSERT(0, "Framebuffer OK");
	}
}


void RenderScene::CleanupStuff() {

	// TODO [Implement] Call this from somewhere:
	// Free up the frame buffer object:
	GLCALL(glDeleteFramebuffers, 1, &g_depth_fbo_id);
}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera) {

#ifdef PLATFORM_IOS
	GLCALL(glGetIntegerv, GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	GLCALL(glBindFramebuffer, GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	renderLists->Draw(camera, false, DISTANCE_FROM_CAMERA_COMPARE_TYPE_ortho_z);
}


void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera,
							  DirectionalLight* directionalLight,
							  std::vector<DirectionalLight*> additionalLights) {

#ifdef PLATFORM_IOS
	GLCALL(glGetIntegerv, GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	std::vector<DirectionalLight*> emptyVector;

	{
		// Switch off blend for depth pass:
    	GLCALL(glDisable, GL_BLEND);

		Camera* depthCamera = ENGINE->GetShadowMap()->GetDepthCamera();
#if defined(DRAWING_DEPTH_BUFFER_CONTENTS)
    	GLCALL(glViewport, 0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
		GLCALL(glBindFramebuffer, GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
#else

		/*
	 	 * Draw to the depth buffer:
	 	 */
		unsigned int depthMap_width, depthMap_height;
		ENGINE->GetShadowMap()->GetDepthMapResolution(depthMap_width, depthMap_height);
		//
		GLCALL(glBindFramebuffer, GL_FRAMEBUFFER, g_depth_fbo_id);
    	GLCALL(glViewport, 0, 0, depthMap_width, depthMap_height);
		GLCALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
		// Depth pass draw:
		ENGINE->GetSceneGraph3D()->SetMainCameraId(depthCamera->GetObject()->GetId());
		renderLists->Draw(depthCamera, nullptr /* no lights in depth pass */, emptyVector /* no lights in depth pass */, true, DISTANCE_FROM_CAMERA_COMPARE_TYPE_perspective);

		// Switch blend back on:
    	GLCALL(glEnable, GL_BLEND);
	}
	

	{
#if !defined(DRAWING_DEPTH_BUFFER_CONTENTS)
		/*
	 	 * Draw again, this time to the screen:
	 	 */
		ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetObject()->GetId());
		GLCALL(glBindFramebuffer, GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
    	GLCALL(glViewport, 0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
		renderLists->Draw(camera, directionalLight, additionalLights, false, DISTANCE_FROM_CAMERA_COMPARE_TYPE_perspective);
#endif
	}

}
