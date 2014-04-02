#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/WaterRenderer.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

WaterRenderer::WaterRenderer() : MeshRenderer() {
	this->init();
}

WaterRenderer::WaterRenderer(Object* object_) : MeshRenderer(object_)  {
	this->init();
}

WaterRenderer::~WaterRenderer() {
	this->destroy();
}

void WaterRenderer::InitMesh(std::string urlOfMesh) {
	MeshRenderer::InitMesh(urlOfMesh);
}

void WaterRenderer::SetScrollingUVs(float uvScrollSpeed) {
	this->scrollingUVs_speed = uvScrollSpeed;
}

void WaterRenderer::SetSecondaryTexture(std::string pathToTexture) {
	this->secondaryTexture = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(pathToTexture);
}

void WaterRenderer::HandleMessage(MessageChunk messageChunk) {
	MeshRenderer::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
	case MESSAGE_TYPE_FRAME_EVENT: {
		this->scrollUVs();
	} break;

	default : {
	} break;
	}
}

void WaterRenderer::Draw() {
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_scrolling_uv_offset)) {
		GLint scrolling_uv_offset_handle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_scrolling_uv_offset);
		GLCALL(glUniform1f, scrolling_uv_offset_handle, this->scrollingUVsOffset);
	}

	MeshRenderer::Draw();

	if (this->secondaryTexture) {
		this->secondaryTexture->Draw(3);
	}
}


void WaterRenderer::scrollUVs() {
	this->scrollingUVsOffset += this->scrollingUVs_speed * ENGINE->GetTimer()->GetDeltaFrameTime();
}

void WaterRenderer::init() {
	this->scrollingUVsOffset  = 0.0f;

	this->scrollingUVs_speed  = 0.1f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void WaterRenderer::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

