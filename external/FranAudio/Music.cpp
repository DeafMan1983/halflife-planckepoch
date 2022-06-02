// FranticDreamer 2022

#include "FranAudio.hpp"
#include "Channel.hpp"
#include "Music.hpp"

#include "Utilities.hpp"
#include "ALWrappers.hpp"

#include "libnyquist/Decoders.h"

nqr::AudioData mus_emptyAudioData = nqr::AudioData();
nqr::NyquistIO mus_loader;

FranAudio::Music::Music()
	: sampleDir("ERR"),
	firstUpdate(true)
{

}

FranAudio::Music::Music(const char* _sample, float _volume) 
	: sampleDir(FranAudio::Utilities::ReturnPlayableDir(_sample, true)), 
	firstUpdate(true)
{

	if (sampleDir == "ERR")
		return;

	PrecacheMusicSingle(sampleDir);

	auto& MusicData = FindPrecachedMusicSingle(sampleDir);

	if (MusicData.samples.empty())
	{
		// Load unsuccessful
		sampleDir = "ERR";
		return;
	}

	FranAudio::Globals::LogMessage(std::string(std::string("Trying to play music: ") + "\"" + sampleDir + "\"" + " - " + std::to_string(MusicData.lengthSeconds) + " | " + std::to_string(MusicData.channelCount) + " | " + std::to_string(MusicData.sampleRate) + " | " + std::to_string(MusicData.samples.size())).c_str());

	FranAudio_AlFunction(alGenBuffers, 1, &sourceBuffer);
	FranAudio_AlFunction(alBufferData, sourceBuffer, GetMusicFormat(MusicData.channelCount), MusicData.samples.data(), MusicData.samples.size() * 4, MusicData.sampleRate);
	
	FranAudio_AlFunction(alGenSources, 1, &sourceHandle);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_PITCH, 1);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, 1.0f);

	FranAudio_AlFunction(alSourcei, sourceHandle, AL_SOURCE_RELATIVE, true);
	
	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, Vector{0, 0, 0});
	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_VELOCITY, Vector{0, 0, 0});
	FranAudio_AlFunction(alSourcei, sourceHandle, AL_LOOPING, AL_FALSE);
	FranAudio_AlFunction(alSourcei, sourceHandle, AL_BUFFER, sourceBuffer);

	FranAudio_AlFunction(alSourcePlay, sourceHandle);

	sourceState = AL_PLAYING;

	FranAudio::Globals::LogMessage(std::string(std::string("Sample ") + "\"" + sampleDir.c_str() + "\"" + " playing.").c_str());

}

bool FranAudio::Music::operator==(const Music& _other)
{
	if (sampleDir != _other.sampleDir)
		return false;

	return true;
}

void FRANAUDIO_API FranAudio::Music::Update(float _mp3volumecvarvalue)
{
	if (sampleDir == "ERR" || sourceState == AL_STOPPED)
	{
		StopMusic();
		return;
	}

	if (firstUpdate)
	{
		firstUpdate = false;

		//FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, 1.0f);
	}

	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, _mp3volumecvarvalue * 0.01f);

	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SOURCE_STATE, &sourceState);
}

void FRANAUDIO_API FranAudio::Music::SetPaused(bool _pause, bool isMenu)
{
	if (sampleDir == "ERR")
		return;

	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SOURCE_STATE, &sourceState);

	if (isMenu)
	{
		isPausedByPauseMenu = _pause;

		if (_pause)
			FranAudio_AlFunction(alSourcePause, sourceHandle);
		else if (sourceState == AL_PAUSED)
			FranAudio_AlFunction(alSourcePlay, sourceHandle);
	}
	else if (!isPausedByPauseMenu)
	{
		if (_pause)
			FranAudio_AlFunction(alSourcePause, sourceHandle);
		else if (sourceState == AL_PAUSED)
			FranAudio_AlFunction(alSourcePlay, sourceHandle);
	}
}

std::string FranAudio::Music::GetDir()
{
	return sampleDir;
}

bool FranAudio::Music::Kill()
{
	if (sampleDir == "ERR")
	{
		return true;
	}

	FranAudio_AlFunction(alSourceStop, sourceHandle);
	FranAudio_AlFunction(alDeleteSources, 1, &sourceHandle);
	FranAudio_AlFunction(alDeleteBuffers, 1, &sourceBuffer);

	sampleDir = "ERR"; // Just in case

	return true;
}

void FRANAUDIO_API FranAudio::Music::PlayMusic(const char* _sample, float _mp3volumecvarvalue)
{
	StopMusic(); // TODO: Fade Out
	CurrentMusic = new Music(_sample, _mp3volumecvarvalue);
}

void FRANAUDIO_API FranAudio::Music::StopMusic()
{
	if (CurrentMusic != nullptr)
	{
		CurrentMusic->Kill();
		delete CurrentMusic;
		CurrentMusic = nullptr;
	}
}

void FRANAUDIO_API FranAudio::Music::SetMusicPaused(bool _pause, bool isMenu)
{
	CurrentMusic->SetPaused(_pause, isMenu);
}

inline bool FranAudio::Music::PrecacheMusic(std::string _dir)
{
	if (MusicsMap.find(_dir) == MusicsMap.end())
	{
		FranAudio::Globals::LogMessage(std::string(std::string("Trying to load ") + "\"" + _dir.c_str() + "\"").c_str());

		nqr::AudioData _sample{};

		mus_loader.Load(&_sample, _dir);

		FranAudio::Globals::LogMessage(std::string(std::string("Loaded: ") + "\"" + _dir.c_str() + "\"" + " - " + std::to_string(_sample.lengthSeconds) + " | " + std::to_string(_sample.channelCount) + " | " + std::to_string(_sample.sampleRate) + " | " + std::to_string(_sample.samples.size())).c_str());

		MusicsMap.insert({_dir, _sample});
	}
	
	return true;
}

inline nqr::AudioData& FranAudio::Music::FindPrecachedMusic(std::string _dir)
{
	if (MusicsMap.find(_dir) == MusicsMap.end())
	{
		return mus_emptyAudioData; // Return empty Music sample
	}
	else
	{
		return MusicsMap.at(_dir);
	}
}


ALenum FranAudio::Music::GetMusicFormat(int _channels, bool _isDouble)
{
	if (FranAudio::Globals::IsFloatFormatSupported() && _channels == 1 && !_isDouble)
		return AL_FORMAT_MONO_FLOAT32;
	else if (FranAudio::Globals::IsFloatFormatSupported() && _channels == 2 && !_isDouble)
		return AL_FORMAT_STEREO_FLOAT32;
	else if (FranAudio::Globals::IsDoubleFormatSupported() && _channels == 1 && _isDouble)
		return AL_FORMAT_MONO_DOUBLE_EXT;
	else if (FranAudio::Globals::IsDoubleFormatSupported() && _channels == 2 && _isDouble)
		return AL_FORMAT_STEREO_DOUBLE_EXT;
	else
		return 0;
}

