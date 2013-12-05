//
//  iOSAudioLoader.mm
//  Created by Matt Kaufmann on 11/24/13.
//

#ifdef PLATFORM_IOS

#import <string>

#import "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoader.h"

long loadAudioFile(const char* audioPath, char** outAudioBytes) {
	NSString* pathNSString = [NSString stringWithUTF8String:audioPath];
	NSData* audioData = [NSData dataWithContentsOfFile:pathNSString];
	
	if (audioData == nil) {
		*outAudioBytes = nullptr;
		return -1;
	}
	
	// Just bulk copy the data
	*outAudioBytes = (char*)audioData.bytes;
	return audioData.length;
}

#endif