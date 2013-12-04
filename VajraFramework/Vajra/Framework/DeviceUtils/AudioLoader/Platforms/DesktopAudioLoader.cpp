#ifdef PLATFORM_DESKTOP

#include "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoader.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
/*
long loadAudioFile(const char* audioPath, char** outAudioBytes) {
	// TODO [Implement] AudioLoader for desktop
	return -1;
}
*/
int file_read_int32_le(char buffer[5], std::ifstream& file) {
	int val = 0;
	file.read(buffer, 4);
	val = ((unsigned char)buffer[0]) | ((unsigned char)buffer[1] << 8) | ((unsigned char)buffer[2] << 16) | ((unsigned char)buffer[3] << 24);
	return val;
}

short file_read_int16_le(char buffer[5], std::ifstream& file) {
	short val = 0;
	file.read(buffer, 2);
	val = ((unsigned char)buffer[0]) | ((unsigned char)buffer[1] << 8);
	return val;
}

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

ALuint loadALAudioFromWAV(const char* audioPath, ALenum* outFormat, ALubyte** outAudioBytes, ALsizei* outLength, ALsizei* outSampleRate) {
	// Initialize OpenAL buffer to store audio data
	ALuint audioBuffer;
	alGenBuffers(1, &audioBuffer);

	// Open audio file for reading
	std::ifstream file;
	file.open(audioPath, std::ios_base::in | std::ios_base::binary);
	if (file.fail()) {
		std::cerr << "Could not open file " << audioPath << std::endl;
		return audioBuffer;
	}

	// Parse the header data
	char tempBuf[5];
	tempBuf[4] = '\0';
	file.read(tempBuf, 4);
	if ((file.gcount() != 4) || (strcmp(tempBuf, "RIFF") != 0)) {
		std::cerr << "Not a WAV file" << std::endl;
		file.close();
		return audioBuffer;
	}

	file_read_int32_le(tempBuf, file);

	file.read(tempBuf, 4);
	if ((file.gcount() != 4) || (strcmp(tempBuf, "WAVE") != 0)) {
		std::cerr << "Not a WAV file" << std::endl;
		file.close();
		return audioBuffer;
	}

	file.read(tempBuf, 4);
	if ((file.gcount() != 4) || (strcmp(tempBuf, "fmt ") != 0)) {
		std::cerr << "Invalid WAV file" << std::endl;
		file.close();
		return audioBuffer;
	}

	// Parse the metadata
	file_read_int32_le(tempBuf, file);
	short format = file_read_int16_le(tempBuf, file);
	short channels = file_read_int16_le(tempBuf, file);
	int sampleRate = file_read_int32_le(tempBuf, file);
	int byteRate = file_read_int32_le(tempBuf, file);
	file_read_int16_le(tempBuf, file);
	short bitsPerSample = file_read_int16_le(tempBuf, file);
	ALenum audioFormat = GetAudioFormat(channels, bitsPerSample);

	file.read(tempBuf, 4);
	// Skip over any other header information
	if (strcmp(tempBuf, "LIST") == 0) {
		int listlen = file_read_int32_le(tempBuf, file);
		file.seekg(listlen, std::ios_base::cur);
	}

	file.read(tempBuf, 4);
	if ((file.gcount() != 4) || (strcmp(tempBuf, "data") != 0)) {
		std::cerr << "Invalid WAV file" << std::endl;
		file.close();
		return audioBuffer;
	}

	// Read in the actual audio data
	int audioDataLength = file_read_int32_le(tempBuf, file);
	unsigned char* audioData = new unsigned char[audioDataLength];
	file.read((char*)audioData, audioDataLength);
	//float duration = float(audioDataLength) / byteRate;

	// Buffer the audio data using OpenAL
	alBufferData(audioBuffer, audioFormat, audioData, audioDataLength, sampleRate);

	// Return the data for the audio
	*outFormat = audioFormat;
	*outAudioBytes = audioData;
	*outLength = audioDataLength;
	*outSampleRate = sampleRate;
	return audioBuffer;
}

#endif
