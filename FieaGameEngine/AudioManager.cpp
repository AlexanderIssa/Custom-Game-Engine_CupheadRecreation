#include "pch.h"
#include "AudioManager.h"

using namespace Fiea::GameEngine;

#define OpenAL_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if (error != AL_NO_ERROR)\
	{\
		std::cerr << "AudioManager: OpenAL Error: " << error << " with call for " << #message << std::endl;\
	}\
}
#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)

#pragma region Manager
void AudioManager::InitializeAudioManager()
{
	// Error check: already initialized
	if (AudioManager::initialized == true)
	{
		std::cerr << "AudioManager: OpenAL Error: " << "Error in AudioManager::InitiazlieAudioManager(): AudioManager is already initialized!" << std::endl;
		throw std::runtime_error("AudioManager: Error in AudioManager::InitiazlieAudioManager(): AudioManager is already initialized!");
	}

	// Manager
	AudioManager::initialized = true;

	// Context
	// find default audio device
	const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice* device = alcOpenDevice(defaultDeviceString);
	std::cout << "AudioManager: Opening OpenAL Device..." << std::endl;
	if (!device)
	{
		std::cerr << "AudioManager: OpenAL Error: " << "Error in AudioManager::InitiazlieAudioManager(): failed to get the default device for OpenAL" << std::endl;
		throw std::runtime_error("AudioManager: Error in AudioManager::InitiazlieAudioManager(): failed to get the default device for OpenAL");
	}
	std::cout << "AudioManager: OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	// create and activate context
	ALCcontext* context = alcCreateContext(device, nullptr);
	std::cout << "AudioManager: Opening OpenAL Context..." << std::endl;
	if (!alcMakeContextCurrent(context))
	{
		std::cerr << "AudioManager: OpenAL Error: " << "Error in AudioManager::InitiazlieAudioManager(): failed to make the OpenAL context the current context" << std::endl;
		throw std::runtime_error("AudioManager: Error in AudioManager::InitiazlieAudioManager(): failed to make the OpenAL context the current context");
	}
	// Check for errors before proceeding
	OpenAL_ErrorCheck("Make context current");

	// Listener
	// create listener
	alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
	ALfloat forwardAndUpVectors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f
	};
	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

	// SFX
	// Set up source pool
	std::cout << "AudioManager: Creating AudioManager mSFXSourcePool of " << AUDIO_SFX_SOURCES << " sources" << std::endl;
	AudioManager::mSFXSourcePool = new std::vector<ALuint>();
	AudioManager::mSFXSourcePool->reserve(AUDIO_SFX_SOURCES);
	for (size_t i = 0; i < AUDIO_SFX_SOURCES; ++i)
	{
		ALuint sourceNumber;
		alec(alGenSources(1, &sourceNumber));
		alec(alSource3f(sourceNumber, AL_POSITION, 0.f, 0.f, 0.f));
		alec(alSource3f(sourceNumber, AL_VELOCITY, 0.f, 0.f, 0.f));
		alec(alSourcef(sourceNumber, AL_PITCH, 1.f));
		alec(alSourcef(sourceNumber, AL_GAIN, 1.f));
		alec(alSourcei(sourceNumber, AL_LOOPING, AL_FALSE));
		mSFXSourcePool->push_back(sourceNumber);
	}
	AudioManager::mCurrentSFXSourceCounter = 0;
	// Create new dictionary
	std::cout << "AudioManager: Creating AudioManager FilenameToSFXBufferMap" << std::endl;
	AudioManager::mFilenameToSFXBufferMap = new std::unordered_map<std::string, ALuint>();

	// BGM
	// Create new dictionaries
	std::cout << "AudioManager: Creating AudioManager BGM maps" << std::endl;
	AudioManager::mFilenameToBGMSourceMap = new std::unordered_map<std::string, ALuint>();
	AudioManager::mBGMSourceToCursorMap = new std::unordered_map<ALuint, size_t>();
	AudioManager::mBGMSourceToWavDataMap = new std::unordered_map<ALuint, ReadWavData>();
}

void AudioManager::CleanupAudioManager()
{
	// Error check: not initialized
	if (AudioManager::initialized == false)
	{
		throw std::runtime_error("AudioManager: Error in AudioManager::CleanupAudioManager(): AudioManager is not initialized!");
	}

	// BGM
	// Delete all sources
	for (const auto& it : *AudioManager::mFilenameToBGMSourceMap)
	{
		std::cout << "AudioManager: Deleting BGM Source associated with " << it.first << std::endl;
		alec(alSourceStop(it.second));
		/* TODO: This leaks AUDIO_BGM_BUFFERS memory! I don't know how to unqueue an unprocessed buffer
		ALuint buffers[AUDIO_BGM_BUFFERS];
		alec(alSourceUnqueueBuffers(it.second, AUDIO_BGM_BUFFERS, &buffers[0]));
		alec(alDeleteBuffers(AUDIO_BGM_BUFFERS, &buffers[0]));
		*/
		alec(alDeleteSources(1, &it.second));
	}
	std::cout << "AudioManager: Deleting AudioManager BGM maps " << std::endl;
	delete AudioManager::mBGMSourceToWavDataMap;
	delete AudioManager::mBGMSourceToCursorMap;
	delete AudioManager::mFilenameToBGMSourceMap;

	// SFX
	// Delete all sources
	std::cout << "AudioManager: Deleting AudioManager mSFXSourcePool" << std::endl;
	alec(alDeleteSources(AUDIO_SFX_SOURCES, &(*AudioManager::mSFXSourcePool)[0]));
	delete AudioManager::mSFXSourcePool;
	// Delete all buffers
	for (const auto& it : *AudioManager::mFilenameToSFXBufferMap)
	{
		std::cout << "AudioManager: Deleting SFX Buffer associated with " << it.first << std::endl;
		alec(alDeleteBuffers(1, &it.second));
	}
	std::cout << "AudioManager: Deleting AudioManager FilenameToSFXBufferMap" << std::endl;
	delete AudioManager::mFilenameToSFXBufferMap;

	// Listener: No action required

	// Context
	ALCcontext* context = alcGetCurrentContext();
	ALCdevice* device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	std::cout << "AudioManager: Destroying OpenAL Context" << std::endl;
	alcDestroyContext(context);
	std::cout << "AudioManager: Closing OpenAL device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	alcCloseDevice(device);

	// Manager
	AudioManager::initialized = false;
}
#pragma endregion

#pragma region SFX
/// @brief Register SFX
/// Registers an SFX into an OpenAL buffer
/// @param filename - filename of the SFX to register
/// @return Returns id of that SFX buffer
size_t AudioManager::RegisterSFX(const std::string& filename)
{
	// Check if already registered
	if (AudioManager::mFilenameToSFXBufferMap->find(filename) != AudioManager::mFilenameToSFXBufferMap->end())
	{
		std::cout << "AudioManager: Tried to re-register SFX: " << filename << std::endl;
		return AudioManager::mFilenameToSFXBufferMap->at(filename);
	}

	// Else, register
	std::cout << "AudioManager: Registering SFX: " << filename << std::endl;

	// Read the wav file
	ReadWavData wavData;
	ReadWavDataFunction(filename, wavData);

	// Create a buffer for it and load it with data
	ALuint buffer;
	alec(alGenBuffers(1, &buffer));
	alec(alBufferData(buffer, 
		wavData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, 
		wavData.pcmData.data(), 
		(ALsizei)wavData.pcmData.size() * AUDIO_BYTES_PER_SAMPLE, 
		(ALsizei)wavData.sampleRate));

	// associate the filename with the buffer
	AudioManager::mFilenameToSFXBufferMap->insert({ filename, buffer });
	return buffer;
}

/// @brief Given a filename, plays the SFX at that filename
/// Will register the filename so it doesn't have to be loaded multiple times
/// Note: Prefer to use PlaySFX2D(size_t) if the id is known to avoid two function overhead calls
/// @param filename - The filename of the SFX
/// @param volume - The volume to play at. Default = 1
/// @return Returns the id of the SFX buffer
size_t AudioManager::PlaySFX2D(const std::string& filename, float volume)
{
	ALuint bufferId = (ALuint)RegisterSFX(filename);
	PlaySFX2D(bufferId, volume);
	return bufferId;
}

/// @brief Given an id, plays the SFX buffer associated with the id
/// @param id - The id of the SFX, as given by RegisterSFX
/// @param volume - The volume to play at. Default = 1
/// @return Returns the id of the SFX buffer
size_t AudioManager::PlaySFX2D(size_t bufferId, float volume)
{
	ALuint source = GetNextAvailableSource();
	alec(alSource3f(source, AL_POSITION, 0.f, 0.f, 0.f));
	alec(alSourcef(source, AL_GAIN, volume));
	alec(alSourcei(source, AL_BUFFER, (ALuint)bufferId));
	alec(alSourcePlay(source));
	return bufferId;
}
#pragma endregion

#pragma region BGM
size_t AudioManager::RegisterBGM(const std::string& filename)
{
	// Check if already registered
	if (AudioManager::mFilenameToBGMSourceMap->find(filename) != AudioManager::mFilenameToBGMSourceMap->end())
	{
		std::cout << "AudioManager: Tried to re-register BGM: " << filename << std::endl;
		return AudioManager::mFilenameToBGMSourceMap->at(filename);
	}

	// Else, register
	std::cout << "AudioManager: Registering BGM: " << filename << std::endl;

	// Read the wav file
	ReadWavData wavData;
	ReadWavDataFunction(filename, wavData);

	// Very rare edge case: BGM file is too small to fill up initial buffers
	if (AUDIO_BGM_BUFFERS * AUDIO_SAMPLES_PER_BGM_BUFFER > wavData.getTotalSamples())
	{
		std::cerr << "AudioManager: BGM file" << filename << " is TOO SMALL" << std::endl;
		throw std::runtime_error("AudioManager: BGM file" + filename + " is TOO SMALL");
	}

	// Create buffers based on global settings
	ALuint buffers[AUDIO_BGM_BUFFERS];
	alec(alGenBuffers(AUDIO_BGM_BUFFERS, &buffers[0]));

	// Load up these buffers with BGM data
	for (size_t i = 0; i < AUDIO_BGM_BUFFERS; ++i)
	{
		alec(alBufferData(buffers[i],
			wavData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
			(char*)&wavData.pcmData[0] + (i * AUDIO_BYTES_PER_BGM_BUFFER),
			AUDIO_BYTES_PER_BGM_BUFFER,
			wavData.sampleRate));
	}

	// Create a source associated with this BGM. Queue up the preloaded buffers
	ALuint source;
	alec(alGenSources(1, &source));
	alec(alSourcef(source, AL_PITCH, 1.f));
	alec(alSourcef(source, AL_GAIN, 1.f));
	alec(alSourcei(source, AL_LOOPING, AL_FALSE)); // I set LOOPING to false here, but the BGM does loop
	alec(alSourceQueueBuffers(source, AUDIO_BGM_BUFFERS, &buffers[0])); // It just doesn't loop over individual buffers

	// Associate the filename with the source
	AudioManager::mFilenameToBGMSourceMap->insert({ filename, source });
	// Associate the source with the cursor position
	AudioManager::mBGMSourceToCursorMap->insert({ source, AUDIO_BGM_BUFFERS * AUDIO_BYTES_PER_BGM_BUFFER });
	// Associate the source with the ReadWavData
	// TODO: this is a pretty expensive copy operation. Consider using heap memory
	AudioManager::mBGMSourceToWavDataMap->insert({ source, wavData });
	return source;
}
size_t AudioManager::StartBGM(const std::string& filename, float volume)
{
	ALuint sourceId = (ALuint)RegisterBGM(filename);
	StartBGM(sourceId, volume);
	return sourceId;
}
size_t AudioManager::StartBGM(size_t sourceId, float volume)
{
	// Error checking: BGM already playing
	ALint state;
	alec(alGetSourcei((ALuint)sourceId, AL_SOURCE_STATE, &state));
	if (state == AL_PLAYING)
	{
		std::cerr << "AudioManager: BGM with id " << sourceId << " is already playing" << std::endl;
		throw std::runtime_error("AudioManager: BGM with id already playing: " + sourceId);
	}
	alec(alSourcef((ALuint)sourceId, AL_GAIN, (ALfloat)volume));
	alec(alSourcePlay((ALuint)sourceId));
	return sourceId;
}
void AudioManager::UpdateBGMBuffersEachFrame()
{
	// For each BGM, update it and see if buffers need to be re-queued
	for (auto it : *AudioManager::mBGMSourceToCursorMap)
	{
		// prepare variables for code readability
		const ALuint& source = it.first;
		const ALuint& cursor = (ALuint)it.second;
		const ReadWavData& wavData = AudioManager::mBGMSourceToWavDataMap->at(source);
		const ALuint& format = wavData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		const ALsizei sampleRate = wavData.sampleRate;
		const std::vector<audio_t>& soundData = wavData.pcmData;

		// Check if any buffers are finished processing
		ALint buffersProcessed = 0;
		alec(alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed));

		// If buffers were processed, then swap them out
		while (buffersProcessed > 0)
		{
			// Unqueue used buffer
			ALuint buffer;
			alec(alSourceUnqueueBuffers(source, 1, &buffer));

			// array to hold new buffer data
			audio_t* data = new audio_t[AUDIO_SAMPLES_PER_BGM_BUFFER];
			std::memset(data, 0, AUDIO_BYTES_PER_BGM_BUFFER);

			// Get number of bytes to copy into the new buffer, based on if close to end of the wav file
			size_t dataSizeToCopy;
			if (cursor + AUDIO_BYTES_PER_BGM_BUFFER > soundData.size() * sizeof(audio_t)) 
			{ 
				dataSizeToCopy = (soundData.size() * sizeof(audio_t)) - cursor; 
			}
			else 
			{ 
				dataSizeToCopy = AUDIO_BYTES_PER_BGM_BUFFER; 
			}

			// Copy the data and increment the cursor
			std::memcpy(&data[0], (char*)&soundData[0] + cursor, dataSizeToCopy);
			(*AudioManager::mBGMSourceToCursorMap)[source] += dataSizeToCopy;

			// If we didn't completely fill the buffer earlier, then reset cursor and loop BGM
			if (dataSizeToCopy < AUDIO_BYTES_PER_BGM_BUFFER)
			{
				std::memcpy((char*)&data[0] + dataSizeToCopy, (char*)&soundData[0], AUDIO_BYTES_PER_BGM_BUFFER - dataSizeToCopy);
				(*AudioManager::mBGMSourceToCursorMap)[source] = AUDIO_BYTES_PER_BGM_BUFFER - dataSizeToCopy;
			}

			// Fill and Requeue the buffer
			alec(alBufferData(buffer, format, data, AUDIO_BYTES_PER_BGM_BUFFER, sampleRate));
			alec(alSourceQueueBuffers(source, 1, &buffer));
			delete[] data;

			--buffersProcessed;
		}
	}
	/*
	while (buffersProcessed--)
	{

	// actually copy
	std::memcpy(&data[0], (char*)&soundData[0] + cursor, dataSizeToCopy);
	cursor += dataSizeToCopy;

	alec(alBufferData(buffer, format, data, dataSizeInBytes, sampleRate));
	alec(alSourceQueueBuffers(source, 1, &buffer));

	delete[] data;
	}
	*/
}
#pragma endregion

#pragma region HelperFunctions
// Function that reads in wav data from a filename and stores it in a ReadWavData struct
void AudioManager::ReadWavDataFunction(const std::string& filename, ReadWavData& wavData)
{
	// Assert that size of audio_t matches hardcoded drwav_int16
	static_assert(sizeof(audio_t) == sizeof(drwav_int16), "AudioManager: size of audio_t does not match drwav_int16");
	// Read wav file
	drwav_int16* pSampleData = drwav_open_file_and_read_pcm_frames_s16(filename.c_str(), &wavData.channels, &wavData.sampleRate, &wavData.totalPCMFrameCount, nullptr);
	// Error checking
	if (pSampleData == NULL)
	{
		std::cerr << "AudioManager: failed to load audio file: " << filename << std::endl;
		drwav_free(pSampleData, nullptr);
	}
	else if (wavData.getTotalSamples() > drwav_uint64(std::numeric_limits<ALsizei>::max()))
	{
		std::cerr << "AudioManager: too much data in file " << filename << " for ALsizei addressed vector" << std::endl;
		drwav_free(pSampleData, nullptr);
	}
	// Successful. Copy wav file to ReadWavData struct
	else
	{
		wavData.pcmData.resize(size_t(wavData.getTotalSamples()));
		std::memcpy(wavData.pcmData.data(), pSampleData, wavData.pcmData.size() * AUDIO_BYTES_PER_SAMPLE);
		drwav_free(pSampleData, nullptr);
	}
}
// Function that gets the next available source from the source pool
ALuint AudioManager::GetNextAvailableSource()
{
	// Get the next source from the pool
	++mCurrentSFXSourceCounter; mCurrentSFXSourceCounter %= AUDIO_SFX_SOURCES;
	ALuint result = mSFXSourcePool->at(mCurrentSFXSourceCounter);
	// Warning check: that source was still playing
	ALint state;
	alec(alGetSourcei(result, AL_SOURCE_STATE, &state));
	if (state == AL_PLAYING)
	{
		std::cerr << "AudioManager: Warning: SFX source was still playing in GetNextAvailableSource()" << std::endl;
	}
	return result;
}
#pragma endregion


