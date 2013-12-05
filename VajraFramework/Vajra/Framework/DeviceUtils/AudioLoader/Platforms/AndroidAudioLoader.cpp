#ifdef PLATFORM_ANDROID

#include "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoader.h"
#include "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoaderHelper.h"
#include "Vajra/Utilities/FileUtilities.h"

#include <string>

ALuint loadALAudioFromWAV(const char* audioPath, ALenum* outFormat, ALubyte** outAudioBytes, ALsizei* outLength, ALsizei* outSampleRate) {
	return 0;
}

#endif
