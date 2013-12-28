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

// TODO [Cleanup] Look into platform-specific I/O methods that may allow support for more audio formats.
// If none can be found, the platform-specific .cpp files should be merged into one.

ALuint loadALAudioFromWAV(const char* audioPath, ALenum* outFormat, ALubyte** outAudioBytes, ALsizei* outLength, ALsizei* outSampleRate);

#endif // AUDIOLOADER_H
