// FranticDreamer 2022

#include "PlatformHeaders.h"

#include <vector>
#include <map>

#include "extdll.h" // For some reason, engine includes MUST be in order.
#include "util.h"
#include "cbase.h"
#include "vector.h" // So we *can't include some in the headers because of reasons

#include "FranAudio.hpp"
#include "Sound.hpp"
#include "Channel.hpp"

#include "cl_entity.h"

/*
// Reserved for initialisation
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
*/

// =============
// Globals
// =============

//FranAudio::Channel FranAudio::Globals::channelsArray[MAX_CHANNELS] = {};

float orientation_vec[6] = {}; // This will be storing forward then up vectors

void FranAudio::Globals::Init(const char* sndDir, const char* fallbackSndDir)
{
	soundDir = sndDir;
	fallbackSoundDir = fallbackSndDir;

	logFile.open(std::string(LOG_DIR) + "FranAudioLog.txt");

	//gSoloud = SoloudContext{};
	gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF /* | SoLoud::Soloud::ENABLE_VISUALIZATION*/);
	gSoloud.setGlobalVolume(4);

	Refresh();
}

void FranAudio::Globals::Refresh()
{
	FranAudio::Channel::channelsVec.Clear();

	for (int i = 0; i < MIN_CHANNELS; i++)
	{
		FranAudio::Channel::channelsVec.Append(Channel());
	}

}

void FranAudio::Globals::Shutdown()
{
	// Stop All Sounds in All Channels
	for (auto& channel : FranAudio::Channel::channelsVec)
	{
		for (auto& sound : channel.sounds)
		{
			sound.Kill();
		}
	}

}

const char* FranAudio::Globals::GetModSoundDir()
{
	return soundDir.c_str();
}

const char* FranAudio::Globals::GetFallbackSoundDir()
{
	return fallbackSoundDir.c_str();
}

void FranAudio::Globals::LogMessage(const char* message)
{
	logFile << ">>> " << message << std::endl;
}

FranAudio::SoloudContext& FranAudio::Globals::GetContext()
{
	return FranAudio::Globals::gSoloud;
}


// =============
// API
// =============

void FRANAUDIO_API FranAudio::EmitSound(int _entityIndex, int _channel, const char* _sample, float _volume, float _attenuation, int _flags, int _pitch)
{
	FranAudio::Channel::channelsVec[_channel].sounds.Append(Sound(_entityIndex, _channel, _sample, _volume, _attenuation, _flags, _pitch));
}

void FRANAUDIO_API FranAudio::SetListenerTransform(Vector position, Vector up, Vector forward)
{
	FranAudio::Globals::GetContext().set3dListenerParameters(position[0], position[1], position[2], forward[0], forward[1], forward[2], up[0], up[1], up[2]);
}

void FRANAUDIO_API FranAudio::RefreshSoundPositions()
{
	FranAudio::Globals::GetContext().update3dAudio();
}

// =============
// General Utilities
// =============

void FranAudio::PlaySample(SoloudSound* _snd, Vector _initialPos)
{
	FranAudio::Globals::GetContext().play3d(*_snd, _initialPos[0], _initialPos[1], _initialPos[2]);
}

void FranAudio::SetSoundPosition(SoloudSoundHandle _snd, Vector _pos)
{
	FranAudio::Globals::GetContext().set3dSourcePosition(_snd, _pos[0], _pos[1], _pos[2]);
}