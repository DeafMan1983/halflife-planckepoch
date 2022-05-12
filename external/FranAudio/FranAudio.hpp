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
#include "SoloudTypes.hpp"

// Forward declaration of the CBaseEntity
// is necessary because of some circular dependiencies

class CBaseEntity;
struct cl_entity_s;

namespace FranAudio
{
	constexpr int MIN_CHANNELS = 16;
	//constexpr int MAX_CHANNELS = 512; // Not limited anymore

	constexpr std::string_view LOG_DIR = "C:\\Users\\Frantic\\Documents\\";

	class Channel;

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

		inline static SoloudContext gSoloud;
#pragma warning(pop)
	public:
		static void Init(const char* sndDir, const char* fallbackSndDir);
		static void Refresh();
		static void Shutdown();

		static const char* GetModSoundDir();
		static const char* GetFallbackSoundDir();

		static void LogMessage(const char* message);

		static SoloudContext& GetContext();
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

	// Applies set positions.
	void FRANAUDIO_API RefreshSoundPositions();

	// Plays a given sample on current context
	void PlaySample(SoloudSound* _snd, Vector _initialPos = Vector(0, 0, 0));

	// Set position of an 3D sound. Need to refresh sounds.
	void SetSoundPosition(SoloudSoundHandle _snd, Vector _pos);
	
};

