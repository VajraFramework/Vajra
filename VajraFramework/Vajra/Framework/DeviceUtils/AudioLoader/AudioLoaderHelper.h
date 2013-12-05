//
//  AudioLoaderHelper.h
//  Created by Matt Kaufmann on 12/04/13.
//

#ifndef AUDIOLOADERHELPER_H
#define AUDIOLOADERHELPER_H

#if PLATFORM_IOS
#include <OpenAL/al.h>
#else
#include "Libraries/openal/headers/al.h"
#endif

ALenum GetAudioFormat(short channels, short bitsPerSample);

#endif // AUDIOLOADERHELPER_H
