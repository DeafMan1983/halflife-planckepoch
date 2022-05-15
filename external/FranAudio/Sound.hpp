// FranticDreamer 2022
#pragma once

#include <map>

#include "FranAudioAPI.hpp"
#include "FranAudio.hpp"

#include "cl_entity.h"
#include "com_model.h"

#include "FranVector.hpp"

#include "SDL2/SDL.h"

namespace FranAudio
{
	enum class ChannelType : unsigned int;

	struct SoundSample
	{
		SDL_AudioSpec info;
		Uint8* data;
		Uint32 length;

		ALenum format;
		
		SoundSample() : data(nullptr), length(0), format(0){};
	};

	class Sound
	{
	private:
		ALuint sourceHandle;
		ALint sourceState;
		ALuint sourceBuffer;

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

		void FRANAUDIO_API SetPaused(bool _pause, bool isMenu = false);

		void Kill();

		// Entity index of owner
		int FRANAUDIO_API EntIndex();

		// ========
		// Static Members
		// ========

		inline static FRANAUDIO_API FranUtils::FranVector<Sound> SoundsVector;

		inline static std::map<std::string, SoundSample> SoundsMap;

		inline static bool PrecacheSound(std::string _dir);

		inline static SoundSample FindPrecachedSound(std::string _dir);
	};
};
