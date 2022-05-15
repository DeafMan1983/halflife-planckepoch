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

#include "ALWrappers.hpp"
#include "Utilities.hpp"

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

float orientation_vec[6] = {}; // This will be storing forward then up vectors

void FranAudio::Globals::Init(const char* sndDir, const char* fallbackSndDir)
{
	soundDir = sndDir;
	fallbackSoundDir = fallbackSndDir;

	logFile.open(std::string(LOG_DIR) + "FranAudioLog.txt");

	openALDevice = alcOpenDevice(nullptr);
	if (!openALDevice)
	{
		LogMessage("ERR: OpenAL init - device error!!");
	}

	if (!FranAudio_AlcFunction(alcCreateContext, mainContext, openALDevice, openALDevice, nullptr) || !mainContext)
	{
		LogMessage("ERR: OpenAL init - context error!!");
	}

	ALCboolean contextMadeCurrent = false;
	if (!FranAudio_AlcFunction(alcMakeContextCurrent, contextMadeCurrent, openALDevice, mainContext) || contextMadeCurrent != ALC_TRUE)
	{
		LogMessage("ERR: OpenAL init - context applying error!!");
	}

	FranAudio_AlFunction(alListenerf, AL_GAIN, 1.0f);

	Refresh();
}

void FranAudio::Globals::Refresh()
{
	FranAudio::Sound::SoundsVector.Clear();
}

void FranAudio::Globals::Shutdown()
{
	// Stop All Sounds in All Channels
	for (auto& sound : FranAudio::Sound::SoundsVector)
	{
		sound.Kill();
	}

	//for (auto& [dir, sound] : FranAudio::Sound::SoundsMap)
	//{
		//SDL_FREESOUND HERE MAYBE IDK
	//}

	ALCboolean closedt, contextt = false;

	FranAudio_AlcFunction(alcMakeContextCurrent, contextt, openALDevice, nullptr);
	FranAudio_AlcFunction(alcDestroyContext, openALDevice, mainContext);

	FranAudio_AlcFunction(alcCloseDevice, closedt, openALDevice, openALDevice);
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

ALCcontext* FranAudio::Globals::GetContext()
{
	return FranAudio::Globals::mainContext;
}


// =============
// API
// =============

void FRANAUDIO_API FranAudio::EmitSound(int _entityIndex, int _channel, const char* _sample, float _volume, float _attenuation, int _flags, int _pitch)
{
	if (_flags != 0)
	{
		auto tempdir = FranAudio::Utilities::ReturnPlayableDir(_sample);
		for (auto& sound : FranAudio::Sound::SoundsVector)
		{
			if (tempdir == sound.GetDir() && _entityIndex == sound.EntIndex())
			{
				if (_flags & SND_STOP)
					sound.Kill();
				if (_flags & SND_CHANGE_VOL)
					sound.SetVolume(_volume);
				if (_flags & SND_CHANGE_PITCH)
					sound.SetPitch(_pitch);

				return;
			}
		}
		return;
	}

	FranAudio::Sound::SoundsVector.Append(Sound(_entityIndex, _channel, _sample, _volume, _attenuation, _flags, _pitch));
}

void FRANAUDIO_API FranAudio::SetListenerTransform(Vector position, Vector up, Vector forward)
{
	FranAudio_AlFunction(alListenerfv, AL_POSITION, position);

	orientation_vec[0] = forward.x;
	orientation_vec[1] = forward.y;
	orientation_vec[2] = forward.z;
	orientation_vec[3] = up.x;
	orientation_vec[4] = up.y;
	orientation_vec[5] = up.z;

	FranAudio_AlFunction(alListenerfv, AL_ORIENTATION, orientation_vec);
}

// =============
// General Utilities
// =============