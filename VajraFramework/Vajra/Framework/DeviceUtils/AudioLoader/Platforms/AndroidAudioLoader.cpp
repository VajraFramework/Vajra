#ifdef PLATFORM_ANDROID

#include <string>

#include "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoader.h"

ALuint loadALAudioFromWAV(const char* audioPath, ALenum* outFormat, ALubyte** outAudioBytes, ALsizei* outLength, ALsizei* outSampleRate) {
	return 0;
}

#endif
