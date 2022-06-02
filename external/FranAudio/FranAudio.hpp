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
#include "AL/alext.h"

// Forward declaration of the CBaseEntity
// is necessary because of some circular dependiencies

class CBaseEntity;
struct cl_entity_s;

namespace FranAudio
{
	namespace SoundSpawnFlags 
	{
		// Fake scoped enum ehehehe

		enum SoundSpawnFlags_e
		{
			NoMouthMovement = (1 << 1), // Don't move monsters' mouths
		};
	};

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

		inline static std::string modDir;
		inline static std::string fallbackDir;

		inline static std::ofstream logFile;

		inline static ALCcontext* mainContext;

		inline static ALCdevice* openALDevice;

		inline static bool al_isFloatSupported;
		inline static bool al_isDoubleSupported;
		inline static bool alc_isHRTFSupported;

#pragma warning(pop)
	public:
		static void Init(const char* _sndDir, const char* _fallbackSndDir, const char* _modDir, const char* _fallbackDir);
		static void Refresh();
		static void Shutdown();

		static const char* GetModSoundDir();
		static const char* GetFallbackSoundDir();

		static const char* GetModDir();
		static const char* GetFallbackDir();

		static void LogMessage(const char* message);

		//Unused
		static ALCcontext* GetMainContext();

		static bool SetCurrentContext(ALCcontext* _cont);

		static bool IsFloatFormatSupported();
		static bool IsDoubleFormatSupported();
		static bool IsHRTFSupported();
	};

	// Creates a sound
	void FRANAUDIO_API EmitSound(int _entityIndex,
		int _channel,
		const char* _sample,
		float _volume,
		float _attenuation,
		int _flags,
		int _pitch,
		int _spawnflags);

	void FRANAUDIO_API SetListenerTransform(Vector position, Vector up, Vector forward);
	
};

