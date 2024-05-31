#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "dr_wav.h"

#include "glm/glm.hpp"

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace Fiea::GameEngine
{
	// Global settings
	typedef uint16_t audio_t;
	constexpr size_t AUDIO_BYTES_PER_SAMPLE = sizeof(audio_t) / sizeof(char);
	constexpr size_t AUDIO_BGM_BUFFERS = 4;
	constexpr size_t AUDIO_SAMPLES_PER_BGM_BUFFER = 65536;
	constexpr size_t AUDIO_BYTES_PER_BGM_BUFFER = AUDIO_SAMPLES_PER_BGM_BUFFER * AUDIO_BYTES_PER_SAMPLE;
	constexpr size_t AUDIO_SFX_SOURCES = 25;

	// struct to hold data about a Wav file
	struct ReadWavData
	{
		unsigned int channels = 0;
		unsigned int sampleRate = 0;
		drwav_uint64 totalPCMFrameCount = 0;
		std::vector<audio_t> pcmData;
		drwav_uint64 getTotalSamples() { return totalPCMFrameCount * channels; }
	};

	// AudioManager singleton class
	class AudioManager
	{
	public:
		// Manager
		static void InitializeAudioManager();
		static void CleanupAudioManager();

		// SFX
		static size_t RegisterSFX(const std::string& filename);
		static size_t PlaySFX2D(const std::string& filename, float volume = 1);
		static size_t PlaySFX2D(size_t bufferId, float volume = 1);

		/*
		static size_t PlaySFXAtLocation(const std::string& filename, float volume = 1);
		static size_t PlaySFXAtLocation(const std::string& filename, glm::vec4 position, float volume = 1);
		static size_t PlaySFXAtLocation(const std::string& filename, float x, float y, float z, float volume = 1);
		static size_t PlaySFXAtLocation(size_t bufferId, float volume = 1);
		static size_t PlaySFXAtLocation(size_t bufferId, glm::vec4 position, float volume = 1);
		static size_t PlaySFXAtLocation(size_t bufferId, float x, float y, float z, float volume = 1);
		*/

		// BGM
		static size_t RegisterBGM(const std::string& filename);
		static size_t StartBGM(const std::string& filename, float volume = 1);
		static size_t StartBGM(size_t sourceId, float volume = 1);
		static void UpdateBGMBuffersEachFrame();
		/*
		static void StopBGM(std::string filename);
		static void StopBGM(size_t sourceId);
		static void StopAllBGM();
		*/


	private:
		// Manager
		inline static bool initialized;
		//SFX
		// Vector of sfxSources to use for playing back audio, and a counter to determine which source to use
		inline static std::vector<ALuint>* mSFXSourcePool;
		inline static size_t mCurrentSFXSourceCounter;
		// Map of filename strings to their associated buffers (used for SFX)
		inline static std::unordered_map<std::string, ALuint>* mFilenameToSFXBufferMap;
		// BGM
		// Map of filename strings to their associated BGM sources
		inline static std::unordered_map<std::string, ALuint>* mFilenameToBGMSourceMap;
		// Map of BGM sources to their current cursors
		inline static std::unordered_map<ALuint, size_t>* mBGMSourceToCursorMap;
		// Map of BGM sources to their WavData
		inline static std::unordered_map<ALuint, ReadWavData>* mBGMSourceToWavDataMap;

	private:
		// Helper functions
		static void ReadWavDataFunction(const std::string& filename, ReadWavData& monoData);
		static ALuint GetNextAvailableSource();
	};

}