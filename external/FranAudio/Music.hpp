// FranticDreamer 2022
#pragma once

#include <map>

#include "FranAudioAPI.hpp"
#include "FranAudio.hpp"

#include "cl_entity.h"
#include "com_model.h"

#include "FranVector.hpp"

#include "SDL2/SDL.h"

#include "libnyquist/Common.h"

namespace FranAudio
{
	//constexpr int defaultSampleRate = 22050; // 48000? 44100?
	//constexpr int defaultChannelCount = 2;

	class Music
	{
	protected:
		ALuint sourceHandle;
		ALint sourceState;
		ALuint sourceBuffer;

		// Half Life Music Data Structure
		std::string sampleDir; // If "ERR", that means this Music is dead.

		bool isPausedByPauseMenu;

		bool firstUpdate; // Will update for the first time?

	public:
		Music();

		Music(const char* _sample,
			float _volume);

		bool operator==(const Music& _other);

		// For refreshing
		void FRANAUDIO_API Update(float _mp3volumecvarvalue);

		virtual bool PrecacheMusicSingle(std::string _dir) { return PrecacheMusic(_dir); };
		virtual nqr::AudioData& FindPrecachedMusicSingle(std::string _dir) { return FindPrecachedMusic(_dir); };

		void FRANAUDIO_API SetPaused(bool _pause, bool isMenu = false);

		std::string GetDir();

		bool Kill();

		// ========
		// Static Members
		// ========

		// Maybe should be a smart pointer?
		inline static FRANAUDIO_API Music* CurrentMusic;

		inline static std::map<std::string, nqr::AudioData> MusicsMap;

		static void FRANAUDIO_API PlayMusic(const char* _sample, float _mp3volumecvarvalue);
		static void FRANAUDIO_API StopMusic();
		static void FRANAUDIO_API SetMusicPaused(bool _pause, bool isMenu = false);
		
		static bool PrecacheMusic(std::string _dir);

		static nqr::AudioData& FindPrecachedMusic(std::string _dir);

		static ALenum GetMusicFormat(int _channels, bool _isDouble = false);
	};
};
