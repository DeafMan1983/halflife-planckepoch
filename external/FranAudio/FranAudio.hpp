// FranticDreamer 2022
#pragma once

#include "PlatformHeaders.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <map>

#include "extdll.h" // For some reason, engine includes MUST be in order.
#include "util.h"
#include "cbase.h"
#include "vector.h" // So we *can't include some in the headers because of reasons

#include "FranAudioAPI.hpp"

#include "AL/al.h"
#include "AL/alc.h"

// Forward declaration of the CBaseEntity
// is necessary because of some circular dependiencies

class CBaseEntity;
struct cl_entity_s;

namespace FranAudio
{
	//constexpr int FA_MIN_CHANNELS = 16; 
	//constexpr int FA_MAX_CHANNELS = 128;

	constexpr std::string_view LOG_DIR = "C:\\Users\\Frantic\\Documents\\";

	class FRANAUDIO_API Globals
	{
	private:
#pragma warning(push)
#pragma warning(disable : 4251)
		// Disable export warnings, 
		// since members are private anyway

		inline static std::string soundDir;
		inline static std::string fallbackSoundDir;

		inline static std::ofstream logFile;

		inline static ALCcontext* mainContext;

		inline static ALCdevice* openALDevice;

#pragma warning(pop)
	public:
		static void Init(const char* sndDir, const char* fallbackSndDir);
		static void Refresh();
		static void Shutdown();

		static const char* GetModSoundDir();
		static const char* GetFallbackSoundDir();

		static void LogMessage(const char* message);

		static ALCcontext* GetContext();
	};

	// Creates a sound
	void FRANAUDIO_API EmitSound(int _entityIndex,
		int _channel,
		const char* _sample,
		float _volume,
		float _attenuation,
		int _flags,
		int _pitch);

	void FRANAUDIO_API SetListenerTransform(Vector position, Vector up, Vector forward);
	
};

