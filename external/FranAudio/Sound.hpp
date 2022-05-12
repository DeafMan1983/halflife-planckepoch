// FranticDreamer 2022
#pragma once

#include "FranAudioAPI.hpp"
#include "FranAudio.hpp"

#include "cl_entity.h"
#include "com_model.h"

#include "SoloudTypes.hpp"

namespace FranAudio
{
	enum class ChannelType : unsigned int;

	class Sound
	{
	private:
		SoloudSound sampleData;
		SoloudSoundHandle sampleHandle;

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

	};
};
