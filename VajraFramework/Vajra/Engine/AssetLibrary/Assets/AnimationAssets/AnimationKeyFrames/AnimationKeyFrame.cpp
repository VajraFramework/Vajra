#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AnimationKeyFrame::AnimationKeyFrame() {
    this->init();
}

AnimationKeyFrame::~AnimationKeyFrame() {
    this->destroy();
}

void AnimationKeyFrame::init() {
	this->time = 0.0f;
}

void AnimationKeyFrame::destroy() {
}
