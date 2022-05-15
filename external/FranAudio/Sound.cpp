// FranticDreamer 2022

#include "FranAudio.hpp"
#include "Channel.hpp"
#include "Sound.hpp"

#include "Utilities.hpp"
#include "ALWrappers.hpp"

FranAudio::Sound::Sound()
	: entity(INT32_MAX),
	  channelType(ChannelType::Auto),
	  sampleDir("ERR"),
	  volume(0),
	  attenuation(0),
	  flags(0),
	  pitch(0)
{

}

FranAudio::Sound::Sound(int _entityIndex, int _channel, const char* _sample, float _volume, float _attenuation, int _flags, int _pitch) 
	: entity(_entityIndex), 
	channelType((ChannelType)_channel), 
	sampleDir(FranAudio::Utilities::ReturnPlayableDir(_sample)), 
	volume(_volume), 
	attenuation(_attenuation), 
	flags(_flags), 
	pitch(_pitch)
{

	if (sampleDir == "ERR")
		return;

	PrecacheSound(sampleDir);

	auto& sound = FindPrecachedSound(sampleDir);

	if (sound.data == nullptr)
	{
		// Load unsuccessful
		sampleDir = "ERR";
		return;
	}

	FranAudio_AlFunction(alGenBuffers, 1, &sourceBuffer);
	FranAudio_AlFunction(alBufferData, sourceBuffer, sound.format, sound.data, sound.length, sound.info.freq);

	FranAudio_AlFunction(alGenSources, 1, &sourceHandle);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_PITCH, 1);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, 1.0f);
	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, Vector{0, 0, 0});
	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_VELOCITY, Vector{0, 0, 0});
	FranAudio_AlFunction(alSourcei, sourceHandle, AL_LOOPING, AL_FALSE);
	FranAudio_AlFunction(alSourcei, sourceHandle, AL_BUFFER, sourceBuffer);

	FranAudio_AlFunction(alSourcePlay, sourceHandle);

	sourceState = AL_PLAYING;

	FranAudio::Globals::LogMessage(std::string(std::string("Sample ") + "\"" + sampleDir.c_str() + "\"" + " playing.").c_str());

}

bool FranAudio::Sound::operator==(const Sound& _other)
{
	if (entity != _other.entity)
		return false;

	if (channelType != _other.channelType)
		return false;

	if (sampleDir != _other.sampleDir)
		return false;

	if (volume != _other.volume)
		return false;

	if (attenuation != _other.attenuation)
		return false;

	if (flags != _other.flags)
		return false;

	if (pitch != _other.pitch)
		return false;

	return true;
}

void FRANAUDIO_API FranAudio::Sound::Update(cl_entity_t* _ent)
{
	if (sampleDir == "ERR" || _ent == nullptr || sourceState == AL_STOPPED /* || !audioSource || audioSource.isPending()*/)
	{
		Kill();
		return;
	}


	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SOURCE_STATE, &sourceState);

	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, _ent->origin);

	//if (_ent->model)
		//sampleHandle.setRadius(_ent->model->radius);
}

void FRANAUDIO_API FranAudio::Sound::SetPaused(bool _pause, bool isMenu)
{
	if (sampleDir == "ERR")
		return;

	if (isMenu)
	{
		isPausedByPauseMenu = _pause;
		//_pause ? audioSource.pause() : audioSource.resume();
	}
	else if (!isPausedByPauseMenu)
	{
		//_pause ? audioSource.pause() : audioSource.resume();
	}
}

void FRANAUDIO_API FranAudio::Sound::SetVolume(float _volume)
{
	volume = _volume;
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, _volume);
}

void FRANAUDIO_API FranAudio::Sound::SetPitch(float _pitch)
{
	pitch = _pitch;
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_PITCH, _pitch / 100);
}

const char* FRANAUDIO_API FranAudio::Sound::GetDir()
{
	return sampleDir.c_str();
}

void FranAudio::Sound::Kill()
{
	if (sampleDir == "ERR")
	{
		FranAudio::Sound::SoundsVector.PopIndex(FranAudio::Sound::SoundsVector.Find(*this));
		return;
	}
	FranAudio_AlFunction(alSourceStop, sourceHandle);
	FranAudio_AlFunction(alDeleteSources, 1, &sourceHandle);
	FranAudio_AlFunction(alDeleteBuffers, 1, &sourceBuffer);

	//sampleDir = "ERR";

	// Remove this sound from the list;
	size_t indexToPop = FranAudio::Sound::SoundsVector.Find(*this);
	if (indexToPop != SIZE_MAX || indexToPop < FranAudio::Sound::SoundsVector.Size())
		FranAudio::Sound::SoundsVector.PopIndex(indexToPop);
}

int FRANAUDIO_API FranAudio::Sound::EntIndex()
{
	return entity;
}

inline bool FranAudio::Sound::PrecacheSound(std::string _dir)
{
	if (SoundsMap.find(_dir) == SoundsMap.end())
	{
		FranAudio::Globals::LogMessage(std::string(std::string("Trying to load ") + "\"" + _dir.c_str() + "\"").c_str());

		SoundSample _sample{};

		/* Load the WAV */
		if (SDL_LoadWAV(_dir.c_str(), &_sample.info, &_sample.data, &_sample.length) == NULL)
		{
			FranAudio::Globals::LogMessage(std::string(std::string("Error loading sound: ") + "\"" + _dir.c_str() + "\"").c_str());
			return false;
		}

		if (_sample.info.channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_MONO8;
		else if (_sample.info.channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_MONO16;
		else if (_sample.info.channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_STEREO8;
		else if (_sample.info.channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_STEREO16;

		SoundsMap.insert({_dir, _sample});
	}
	
	return true;
}

inline FranAudio::SoundSample FranAudio::Sound::FindPrecachedSound(std::string _dir)
{
	if (SoundsMap.find(_dir) == SoundsMap.end())
	{
		return SoundSample(); // Return empty sound sample
	}
	else
	{
		return SoundsMap.at(_dir);
	}
}



