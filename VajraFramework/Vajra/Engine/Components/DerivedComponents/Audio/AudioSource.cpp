//
//  AudioSource.cpp
//  Created by Matt Kaufmann on 11/24/13.
//

#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

#include <algorithm>

unsigned int AudioSource::componentTypeId = COMPONENT_TYPE_ID_AUDIO_SOURCE;

// Constructors
AudioSource::AudioSource() : Component() {
	init();
}

AudioSource::AudioSource(Object* object_) : Component(object_) {
	init();
}

// Destructor
AudioSource::~AudioSource() {
	destroy();
}

void AudioSource::init() {
	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
	this->player = ENGINE->GetAudioManager()->RequestAudioPlayer(this->GetObject()->GetId());
	this->player->SetVolume(this->volume);
	this->player->SetPlaybackSpeed(this->playbackSpeed);
	SetSourceIs3D(false);
	SetPlayOnlyWhenVisible(false);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_CAMERA_CHANGED         , this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void AudioSource::destroy() {
	Stop();
	this->loadedAssets.clear();
	ENGINE->GetAudioManager()->ReturnAudioPlayer(this->GetObject()->GetId(), this->player);
}

void AudioSource::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_CAMERA_CHANGED:
			onCameraChanged();
			break;

		case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
			onTransformChanged();
			break;
	}
}

// Mutators
void AudioSource::LoadAudioClip(std::string key, std::string assetName) {
	this->loadedAssets[key] = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
}

void AudioSource::SetAudioClip(std::string key) {
	auto iter = this->loadedAssets.find(key);
	ASSERT(iter != this->loadedAssets.end(), "AudioSource: Contains clip with id %s", key.c_str());
	if (iter != this->loadedAssets.end()) {
		this->player->SetAudioClip(iter->second);
	}
	else {
		this->player->SetAudioClip("");
	}
}

void AudioSource::SetVolume(float vol) {
	this->volume = vol;
	this->player->SetVolume(vol);
}

void AudioSource::SetPlaybackSpeed(float speed)  { this->player->SetPlaybackSpeed(speed); }

void AudioSource::SetLooping(bool loop) {
	this->player->SetLooping(loop);
}

void AudioSource::SetSourceIs3D(bool is3D) {
	// Remember that an AudioSource being 3D only matters if the AudioListener is set to use 3D sounds!
	if (is3D) {
		this->player->SetPositionIsRelative(false);
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		this->player->SetPosition(trans->GetPositionWorld());
	}
	else {
		// If the source is not 3D, it will act as though it is playing from the listener's position.
		this->player->SetPositionIsRelative(true);
		this->player->SetPosition(0.0f, 0.0f, 0.0f);
	}
	this->positionalAudio = is3D;
}

void AudioSource::SetPlayOnlyWhenVisible(bool vis) {
	this->playOnlyWhenVisible = vis;
}

// Other methods
void AudioSource::Play() {
	this->player->Play();
}

void AudioSource::Play(std::string key, bool loop/*= false*/) {
	auto iter = this->loadedAssets.find(key);
	if (iter != this->loadedAssets.end()) {
		this->player->SetAudioClip(iter->second);
		this->SetLooping(loop);
		Play();
	}
}

void AudioSource::Pause() {
	this->player->Pause();
}

void AudioSource::Stop() {
	this->player->Stop();
}

void AudioSource::Stop(float fadeout) {
	this->player->Stop(fadeout);
}

void AudioSource::onCameraChanged() {
	checkVisibility();
}

void AudioSource::onTransformChanged() {
	if (this->positionalAudio) {
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		glm::vec3 pos = trans->GetPositionWorld();
		this->player->SetPosition(pos);
	}
	checkVisibility();
}

void AudioSource::checkVisibility() {
	if (this->playOnlyWhenVisible) {
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		glm::vec3 pos = trans->GetPositionWorld();
		GameObject* gObj = dynamic_cast<GameObject*>(this->GetObject());
		float tolerance = 4.0f; // TODO [Hack] Magic number
		if (gObj->GetParentSceneGraph()->GetMainCamera()->IsPointInFrustum(pos, tolerance)) {
			if (!this->isVisible) {
				this->player->SetVolume(this->volume);
				this->isVisible = true;
			}
		}
		else if (this->isVisible) {
			this->player->SetVolume(0.0f);
			this->isVisible = false;
		}
	}
}
