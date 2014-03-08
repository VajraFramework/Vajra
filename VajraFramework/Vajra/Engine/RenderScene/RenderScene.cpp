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

#define SCREEN_FRAME_BUFFER 0

#define DEPTH_TEXTURE_W 1024
#define DEPTH_TEXTURE_H 1024

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

	// Create a frame buffer object:
	glGenFramebuffers(1, &g_fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);

	// Create a depth texture:
    glGenTextures(1, &ENGINE->GetShadowMap()->depthTexture);
    checkGlError("glGenTextures");
	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    checkGlError("glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


	// glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH,  DEPTH_TEXTURE_W);
	// glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, DEPTH_TEXTURE_H);

	// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		ASSERT(0, "Framebuffer OK");
	}
}


void RenderScene::CleanupStuff() {

	// TODO [Implement] Call this from somewhere:
	// Free up the frame buffer object:
	glDeleteFramebuffers(1, &g_fbo_id);
}

void DEBUG_DrawShadowMap() {
#if 0
	// Render only on a corner of the window (or we we won't see the real rendering...)
	glViewport(0,0,128,128);

	// Use our shader
	glUseProgram(quad_programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(texID, 0);

	glUniform1f(timeID, (float)(glfwGetTime()*10.0f) );

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(quad_vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		quad_vertexPosition_modelspaceID, // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangles !
	// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
	glDrawArrays(GL_TRIANGLES, 0, 6); // From index 0 to 6 -> 2 triangles
	glDisableVertexAttribArray(quad_vertexPosition_modelspaceID);
#endif

}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera) {

	ENGINE->GetShadowMap()->SetDepthCamera(camera);

	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	renderLists->Draw(camera, false);
}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera,
							  DirectionalLight* directionalLight,
							  std::vector<DirectionalLight*> additionalLights) {



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
	// Setup more camera properties here:
	// ```````````````````````````````````````````````````````````````````````````````````````````````````

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
    glViewport(0, 0, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);
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
    glViewport(0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderLists->Draw(camera, directionalLight, additionalLights, false);

	DEBUG_DrawShadowMap();
}
