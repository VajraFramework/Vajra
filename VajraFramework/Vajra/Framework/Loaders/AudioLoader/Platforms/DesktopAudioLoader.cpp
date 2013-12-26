#ifdef PLATFORM_DESKTOP

#include "Vajra/Framework/Loaders/AudioLoader/AudioLoader.h"
#include "Vajra/Framework/Loaders/AudioLoader/AudioLoaderHelper.h"
#include "Vajra/Utilities/FileUtilities.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

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

	ReadInt32LittleEndianFromFile(file);

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
	ReadInt32LittleEndianFromFile(file);
	short format = ReadInt16LittleEndianFromFile(file);
	short channels = ReadInt16LittleEndianFromFile(file);
	int sampleRate = ReadInt32LittleEndianFromFile(file);
	int byteRate = ReadInt32LittleEndianFromFile(file);
	ReadInt16LittleEndianFromFile(file);
	short bitsPerSample = ReadInt16LittleEndianFromFile(file);
	ALenum audioFormat = GetAudioFormat(channels, bitsPerSample);

	file.read(tempBuf, 4);
	// Skip over any other header information
	if (strcmp(tempBuf, "LIST") == 0) {
		int listlen = ReadInt32LittleEndianFromFile(file);
		file.seekg(listlen, std::ios_base::cur);
	}

	file.read(tempBuf, 4);
	if ((file.gcount() != 4) || (strcmp(tempBuf, "data") != 0)) {
		std::cerr << "Invalid WAV file" << std::endl;
		file.close();
		return audioBuffer;
	}

	// Read in the actual audio data
	int audioDataLength = ReadInt32LittleEndianFromFile(file);
	unsigned char* audioData = new unsigned char[audioDataLength];
	file.read((char*)audioData, audioDataLength);

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
