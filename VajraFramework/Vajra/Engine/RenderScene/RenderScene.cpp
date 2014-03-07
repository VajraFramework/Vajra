#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/RenderScene/RenderScene.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

static GLuint g_fbo_id;

#define SCREEN_FRAME_BUFFER 0

#define DEPTH_TEXTURE_W 1024
#define DEPTH_TEXTURE_H 1024

void RenderScene::SetupStuff() {

	// Create a frame buffer object:
	glGenFramebuffers(1, &g_fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);

	// Create a depth texture:
    glGenTextures(1, &ENGINE->GetShadowMap()->depthTexture);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, ENGINE->GetShadowMap()->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    checkGlError("glTexImage2D");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

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

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera) {

	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	renderLists->Draw(camera);
}

void RenderScene::RenderScene(RenderLists* renderLists, Camera* camera,
							  DirectionalLight* directionalLight,
							  std::vector<DirectionalLight*> additionalLights) {

	/*
	 * Draw to the depth buffer:
	 */
    glViewport(0, 0, DEPTH_TEXTURE_W, DEPTH_TEXTURE_H);
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo_id);
	renderLists->Draw(camera, directionalLight, additionalLights);

	/*
	 * Draw again, this time to the screen:
	 */
    glViewport(0, 0, FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
	glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAME_BUFFER /* default window framebuffer */);
	renderLists->Draw(camera, directionalLight, additionalLights);
}
