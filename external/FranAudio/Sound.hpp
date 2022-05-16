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
	enum class ChannelType : unsigned int;

	//constexpr int defaultSampleRate = 22050; // 48000? 44100?
	//constexpr int defaultChannelCount = 2;

	class Sound
	{
	protected:
		ALuint sourceHandle;
		ALint sourceState;
		ALuint sourceBuffer;

		uint8_t* tempSoundBuffer;

		// Half Life Sound Data Structure

		int entity; // Entindex
		union
		{
			ChannelType channelType;
			unsigned int channelID;
		};
		std::string sampleDir; // If "ERR", that means this sound is dead.
		float volume;
		float attenuation;
		int flags;
		int pitch;

		bool isPausedByPauseMenu;

		bool firstUpdate; // Will update for the first time?

		virtual bool IsMusic() { return false; }; // Not muzak

	public:
		Sound();

		// This constructor is compatible with old EMIT_SOUND macros
		// No std::string here because of DLL exports
		Sound(int _entityIndex,
			int _channel,
			const char* _sample,
			float _volume,
			float _attenuation,
			int _flags,
			int _pitch);

		bool operator==(const Sound& _other);

		// For refreshing vars like position
		void FRANAUDIO_API Update(cl_entity_t* _ent);

		virtual bool PrecacheSoundSingle(std::string _dir) { return PrecacheSound(_dir); };
		virtual nqr::AudioData& FindPrecachedSoundSingle(std::string _dir) { return FindPrecachedSound(_dir); };

		void FRANAUDIO_API SetPaused(bool _pause, bool isMenu = false);

		void SetVolume(float _volume);

		void SetPitch(float _pitch);

		std::string GetDir();

		bool Kill();

		// Entity index of owner
		int FRANAUDIO_API EntIndex();

		// ========
		// Static Members
		// ========

		inline static FRANAUDIO_API FranUtils::FranVector<Sound> SoundsVector;

		inline static std::map<std::string, nqr::AudioData> SoundsMap;

		static void KillAllSounds();
		
		static bool PrecacheSound(std::string _dir);

		static nqr::AudioData& FindPrecachedSound(std::string _dir);

		static ALenum GetFormat(int _channels, nqr::PCMFormat _format);
	};
	/*
	class Music : public Sound
	{
		virtual bool IsMusic() override { return true; }; // Yes muzak

		void FRANAUDIO_API UpdateMusic(cl_entity_t* _ent) override;

		virtual bool PrecacheSoundSingle(std::string _dir) override { return PrecacheMusic(_dir); };
		virtual SoundSample FindPrecachedSoundSingle(std::string _dir) override { return FindPrecachedMusic(_dir); };

		// ========
		// Static Members
		// ========

		//inline static FRANAUDIO_API Music SoundsVector;

		// TODO: We shouldn't keep all the used music cached.
		inline static std::map<std::string, SoundSample> MusicMap; 

		static bool PrecacheMusic(std::string _dir);

		static SoundSample FindPrecachedMusic(std::string _dir);
	};
	*/
};
