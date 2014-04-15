//
//  AudioManager.cpp
//  Created by Matt Kaufmann on 12/04/13.
//

#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/AudioManager/AudioManagerInternal.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>

AudioManager::AudioManager() {
	this->init();
}

AudioManager::~AudioManager() {
	this->destroy();
}

void AudioManager::init() {
	this->internalMgr = new AudioManagerInternal();

	// Set the default listener for OpenAL.
	SetListenerPosition(0.0f, 0.0f, 0.0f);
	SetListenerVelocity(0.0f, 0.0f, 0.0f);
	SetListenerOrientation(ZAXIS, YAXIS);
	SetListenerVolume(1.0f);
}

void AudioManager::destroy() {
	if (this->internalMgr != nullptr) {
		delete this->internalMgr;
		this->internalMgr = nullptr;
	}
}

bool AudioManager::Is3DSoundEnabled() {
	return this->internalMgr->Is3DSoundEnabled();
}

void AudioManager::Enable3DSound() {
	this->internalMgr->Enable3DSound();
}

void AudioManager::Disable3DSound() {
	this->internalMgr->Disable3DSound();
}

void AudioManager::SetListenerPosition(glm::vec3 pos) {
	this->internalMgr->SetListenerPosition(pos);
}

void AudioManager::SetListenerPosition(float x, float y, float z) {
	this->internalMgr->SetListenerPosition(x, y, z);
}

void AudioManager::SetListenerOrientation(glm::quat orient) {
	this->internalMgr->SetListenerOrientation(orient);
}

void AudioManager::SetListenerOrientation(glm::vec3 forward, glm::vec3 up) {
	this->internalMgr->SetListenerOrientation(forward, up);
}

void AudioManager::SetListenerVelocity(glm::vec3 pos) {
	this->internalMgr->SetListenerVelocity(pos);
}

void AudioManager::SetListenerVelocity(float x, float y, float z) {
	this->internalMgr->SetListenerVelocity(x, y, z);
}

void AudioManager::SetListenerVolume(float volume) {
	this->internalMgr->SetListenerVolume(volume);
}

AudioPlayer* AudioManager::RequestAudioPlayer(ObjectIdType objId) {
	if (objId != OBJECT_ID_INVALID) {
		if (ENGINE->GetSceneGraph3D()->GetGameObjectById(objId) != nullptr) {
			this->sources3D.push_back(objId);
		}
		else if (ENGINE->GetSceneGraphUi()->GetGameObjectById(objId) != nullptr) {
			this->sourcesUI.push_back(objId);
		}
		return this->internalMgr->RequestAudioPlayer();
	}
	return nullptr;
}

void AudioManager::ReturnAudioPlayer(ObjectIdType objId, AudioPlayer* player) {
	if (objId != OBJECT_ID_INVALID) {
		this->internalMgr->ReturnAudioPlayer(player);

		auto iter = std::find(this->sources3D.begin(), this->sources3D.end(), objId);
		if (iter != this->sources3D.end()) {
			this->sources3D.erase(iter);
			return;
		}

		iter = std::find(this->sourcesUI.begin(), this->sourcesUI.end(), objId);
		if (iter != this->sourcesUI.end()) {
			this->sources3D.erase(iter);
		}
	}
}

void AudioManager::Pause3dAudio() {
	for (auto iter = this->sources3D.begin(); iter != this->sources3D.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				if (source->IsPlaying()) {
					source->Pause();
				}
			}
		}
	}
}

void AudioManager::PauseUiAudio() {
	for (auto iter = this->sourcesUI.begin(); iter != this->sourcesUI.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraphUi()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				if (source->IsPlaying()) {
					source->Pause();
				}
			}
		}
	}
}

void AudioManager::PauseAllAudio() {
	this->internalMgr->PauseAllAudio();
}

void AudioManager::Resume3dAudio() {
	for (auto iter = this->sources3D.begin(); iter != this->sources3D.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				if (source->IsPaused()) {
					source->Play();
				}
			}
		}
	}
}

void AudioManager::ResumeUiAudio() {
	for (auto iter = this->sourcesUI.begin(); iter != this->sourcesUI.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraphUi()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				if (source->IsPaused()) {
					source->Play();
				}
			}
		}
	}
}

void AudioManager::ResumeAllAudio() {
	this->internalMgr->ResumeAllAudio();
}

void AudioManager::Stop3dAudio() {
	for (auto iter = this->sources3D.begin(); iter != this->sources3D.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				source->Stop();
			}
		}
	}
}

void AudioManager::StopUiAudio() {
	for (auto iter = this->sourcesUI.begin(); iter != this->sourcesUI.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraphUi()->GetGameObjectById(*iter);
		if (gObj != nullptr) {
			AudioSource* source = gObj->GetComponent<AudioSource>();
			if (source != nullptr) {
				source->Stop();
			}
		}
	}
}

void AudioManager::StopAllAudio() {
	this->internalMgr->StopAllAudio();
}
