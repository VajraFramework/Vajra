//
//  AudioLoader.h
//  Created by Matt Kaufmann on 11/24/13.
//

#ifndef AUDIOLOADER_H
#define AUDIOLOADER_H

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#else
#include "Libraries/openal/headers/al.h"
#endif

ALuint loadALAudioFromWAV(const char* audioPath, ALenum* outFormat, ALubyte** outAudioBytes, ALsizei* outLength, ALsizei* outSampleRate);

#endif // AUDIOLOADER_H
