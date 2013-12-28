//
//  AudioLoaderHelper.cpp
//  Created by Matt Kaufmann on 12/04/13.
//

#include "Vajra/Framework/Loaders/AudioLoader/AudioLoaderHelper.h"

ALenum GetAudioFormat(short channels, short bitsPerSample) {
	ALenum format = AL_NONE;
	if (bitsPerSample == 8) {
		if (channels == 1) {
			format = AL_FORMAT_MONO8;
		}
		else if (channels == 2) {
			format = AL_FORMAT_STEREO8;
		}
	}
	else if (bitsPerSample == 16) {
		if (channels == 1) {
			format = AL_FORMAT_MONO16;
		}
		else if (channels == 2) {
			format = AL_FORMAT_STEREO16;
		}
	}
	return format;
}
