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

static GLuint g_fbo_id;

#ifdef PLATFORM_IOS
static GLint g_default_fbo;
#define SCREEN_FRAME_BUFFER g_default_fbo
#else
#define SCREEN_FRAME_BUFFER 0
#endif

#define DEPTH_TEXTURE_W 2048
#define DEPTH_TEXTURE_H 2048

ObjectIdType g_fakeCameraObject_id = OBJECT_ID_INVALID;

// TODO [Hack] Do this better:
GameObject* GetFakeCameraObject() {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(g_fakeCameraObject_id);
	if (gameObject == nullptr) {
		gameObject = new GameObject(ENGINE->GetSceneGraph3D());
		gameObject->AddComponent<Camera>();
		//
		g_fakeCameraObject_id = gameObject->GetId();
	}

	return gameObject;
}

void RenderScene::SetupStuff() {
	
#ifdef PLATFORM_IOS
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	// Create a frame buffer object:
	glGenFramebuffers(1, &g_fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);
	
	// Create a render buffer
	GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

	// Create a depth texture:
    glGenTextures(1, &ENGINE->GetShadowMap()->depthTexture);
    checkGlError("glGenTextures");
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    checkGlError("glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture, 0);
    checkGlError("glFramebufferTexture2D");
	
	// Allocate 16-bit depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H);
    checkGlError("glRenderBufferStorage");
	
	// Attach the render buffer as depth buffer - will be ignored
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  GL_RENDERBUFFER, renderBuffer);
    checkGlError("glFramebufferRenderbuffer");

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ASSERT(0, "Framebuffer OK");
	}
}


void RenderScene::CleanupStuff() {

	// TODO [Implement] Call this from somewhere:
	// Free up the frame buffer object:
	glDeleteFramebuffers(1, &g_fbo_id);
}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera) {

	ENGINE->GetShadowMap()->SetDepthCamera(camera);
	
#ifdef PLATFORM_IOS
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	renderLists->Draw(camera, false);
}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera,
							  DirectionalLight* directionalLight,
							  std::vector<DirectionalLight*> additionalLights) {
	
#ifdef PLATFORM_IOS
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &g_default_fbo);
#endif

	std::vector<DirectionalLight*> emptyVector;

	// TODO [Implement] Position the camera at the position and orientation of the light:
	GameObject* fakeCameraObject = GetFakeCameraObject();
	fakeCameraObject->GetTransform()->SetPosition(-8.0f, 34.0f, 1.0f);
	// fakeCameraObject->GetTransform()->SetOrientation(180.0f inRadians, YAXIS);
	// fakeCameraObject->GetTransform()->Rotate(-85.0f inRadians, XAXIS);
	// fakeCameraObject->GetTransform()->Rotate(25.0f inRadians, YAXIS);
	fakeCameraObject->GetTransform()->LookAt(8.0f, 0.0f, -20.0f);
	Camera* fakeCamera = fakeCameraObject->GetComponent<Camera>();
	fakeCamera->SetCameraType(CAMERA_TYPE_ORTHO);
	fakeCamera->SetOrthoBounds(-10.0f, 50.0f, -10.0f, 50.0f, -10.5f, 100.0f);
	ENGINE->GetSceneGraph3D()->SetMainCameraId(fakeCameraObject->GetId());

	ENGINE->GetShadowMap()->SetDepthCamera(fakeCamera);

	/*
	 * DEBUG: 2 lines below
	 */
    // glViewport(0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
	// glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);

	/*
	 * Draw to the depth buffer:
	 */

	// Switch off blend for depth pass:
    glDisable(GL_BLEND);
    //
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);
    glViewport(0, 0, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderLists->Draw(fakeCamera, nullptr /* no lights in depth pass */, emptyVector /* no lights in depth pass */, true);
	// renderLists->Draw(camera, nullptr /* no lights in depth pass */, emptyVector /* no lights in depth pass */, true);
	//
	// Switch blend back on:
    glEnable(GL_BLEND);
	
	

	/*
	 * Draw again, this time to the screen:
	 */
	ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetObject()->GetId());
	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
    glViewport(0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderLists->Draw(camera, directionalLight, additionalLights, false);

}
