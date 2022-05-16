// FranticDreamer 2022

#include "FranAudio.hpp"
#include "Channel.hpp"
#include "Sound.hpp"

#include "Utilities.hpp"
#include "ALWrappers.hpp"

#include "libnyquist/Decoders.h"

nqr::AudioData snd_emptyAudioData = nqr::AudioData();
nqr::NyquistIO snd_loader;

FranAudio::Sound::Sound()
	: entity(INT32_MAX),
	channelType(ChannelType::Auto),
	sampleDir("ERR"),
	volume(0),
	attenuation(0),
	flags(0),
	pitch(0),
	firstUpdate(true),
	tempSoundBuffer(nullptr)
{
	tempSoundBuffer = nullptr;
}

FranAudio::Sound::Sound(int _entityIndex, int _channel, const char* _sample, float _volume, float _attenuation, int _flags, int _pitch) 
	: entity(_entityIndex), 
	channelType((ChannelType)_channel), 
	sampleDir(FranAudio::Utilities::ReturnPlayableDir(_sample, IsMusic())), 
	volume(_volume), 
	attenuation(_attenuation), 
	flags(_flags), 
	pitch(_pitch),
	firstUpdate(true),
	tempSoundBuffer(nullptr)
{

	if (sampleDir == "ERR")
		return;

	PrecacheSoundSingle(sampleDir);

	auto& sound = FindPrecachedSoundSingle(sampleDir);

	if (sound.samples.empty())
	{
		// Load unsuccessful
		sampleDir = "ERR";
		return;
	}

	FranAudio::Globals::LogMessage(std::string(std::string("Trying to play sample: ") + "\"" + sampleDir + "\"" + " - " + std::to_string(sound.lengthSeconds) + " | " + std::to_string(sound.channelCount) + " | " + std::to_string(sound.sampleRate) + " | " + std::to_string(sound.samples.size())).c_str());

	tempSoundBuffer = new uint8_t[sound.samples.size()];

	nqr::ConvertFromFloat32(tempSoundBuffer, sound.samples.data(), sound.samples.size(), sound.sourceFormat, nqr::DITHER_NONE);

	FranAudio_AlFunction(alGenBuffers, 1, &sourceBuffer);
	FranAudio_AlFunction(alBufferData, sourceBuffer, GetFormat(sound.channelCount, sound.sourceFormat), tempSoundBuffer, sound.samples.size(), sound.sampleRate);

	// This will be used for mouths later. Delete in kill function OR destructor.
	//delete[] tempSoundBuffer; 
	//tempSoundBuffer = nullptr;
	
	FranAudio_AlFunction(alGenSources, 1, &sourceHandle);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_PITCH, 1);
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, volume);

	FranAudio_AlFunction(alSourcei, sourceHandle, AL_SOURCE_RELATIVE, IsMusic());
	
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
	if (sourceState == AL_STOPPED)
		_ent->mouth.mouthopen = 0;

	if (sampleDir == "ERR" || _ent == nullptr || sourceState == AL_STOPPED)
	{
		Kill();
		return;
	}

	if (firstUpdate)
	{
		firstUpdate = false;

		if (_ent->player)
			FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, 0.01f);
	}

	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SOURCE_STATE, &sourceState);

	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, _ent->origin);
	
	if (tempSoundBuffer == nullptr)
	{
		_ent->mouth.mouthopen = 0;
		return;
	}

	ALint sampleOffset;
	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SAMPLE_OFFSET, &sampleOffset);

	auto& sound = FindPrecachedSoundSingle(sampleDir);;

	_ent->mouth.mouthopen = tempSoundBuffer[sampleOffset];

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

void FranAudio::Sound::SetVolume(float _volume)
{
	volume = _volume;
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_GAIN, _volume);
}

void FranAudio::Sound::SetPitch(float _pitch)
{
	pitch = _pitch;
	FranAudio_AlFunction(alSourcef, sourceHandle, AL_PITCH, _pitch / 100);
}

std::string FranAudio::Sound::GetDir()
{
	return sampleDir;
}

bool FranAudio::Sound::Kill()
{
	if (sampleDir == "ERR")
	{
		// Remove this sound from the list;
		size_t indexToPop = FranAudio::Sound::SoundsVector.Find(*this);
		if (indexToPop != SIZE_MAX || indexToPop < FranAudio::Sound::SoundsVector.Size())
		{
			FranAudio::Sound::SoundsVector.PopIndex(indexToPop);
			return true;
		}
		else
		{
			return false;
		}
	}

	if (tempSoundBuffer != nullptr)
		delete[] tempSoundBuffer;

	FranAudio_AlFunction(alSourceStop, sourceHandle);
	FranAudio_AlFunction(alDeleteSources, 1, &sourceHandle);
	FranAudio_AlFunction(alDeleteBuffers, 1, &sourceBuffer);

	//sampleDir = "ERR";

	// Remove this sound from the list;
	size_t indexToPop = FranAudio::Sound::SoundsVector.Find(*this);
	if (indexToPop != SIZE_MAX || indexToPop < FranAudio::Sound::SoundsVector.Size())
	{
		FranAudio::Sound::SoundsVector.PopIndex(indexToPop);
		return true;
	}
	else
	{
		return false;
	}
		
}

int FRANAUDIO_API FranAudio::Sound::EntIndex()
{
	return entity;
}

void FranAudio::Sound::KillAllSounds()
{
	size_t startsize = SoundsVector.Size();
	for (int i = 0; i < startsize; i++)
	{
		if (SoundsVector[i].Kill() == false) // Couldn't kill for some reason
			return;
	}
}

inline bool FranAudio::Sound::PrecacheSound(std::string _dir)
{
	if (SoundsMap.find(_dir) == SoundsMap.end())
	{
		FranAudio::Globals::LogMessage(std::string(std::string("Trying to load ") + "\"" + _dir.c_str() + "\"").c_str());

		nqr::AudioData _sample{};

		//if (snd_loader.IsFileSupported(_dir))
		//{
		//	FranAudio::Globals::LogMessage(std::string(std::string("Error loading sound: ") + "\"" + _dir.c_str() + "\"").c_str());
		//	return false;
		//}

		snd_loader.Load(&_sample, _dir);

		FranAudio::Globals::LogMessage(std::string(std::string("Loaded: ") + "\"" + _dir.c_str() + "\"" + " - " + std::to_string(_sample.lengthSeconds) + " | " + std::to_string(_sample.channelCount) + " | " + std::to_string(_sample.sampleRate) + " | " + std::to_string(_sample.samples.size())).c_str());

		/*
		if (_sample.info.channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_MONO8;
		else if (_sample.info.channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_MONO16;
		else if (_sample.info.channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_STEREO8;
		else if (_sample.info.channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_STEREO16;
		*/
		SoundsMap.insert({_dir, _sample});
	}
	
	return true;
}

inline nqr::AudioData& FranAudio::Sound::FindPrecachedSound(std::string _dir)
{
	if (SoundsMap.find(_dir) == SoundsMap.end())
	{
		return snd_emptyAudioData; // Return empty sound sample
	}
	else
	{
		return SoundsMap.at(_dir);
	}
}


ALenum FranAudio::Sound::GetFormat(int _channels, nqr::PCMFormat _format)
{
	if (_channels == 1 && _format == nqr::PCMFormat::PCM_U8)
		return AL_FORMAT_MONO8;
	else if (_channels == 1 && _format == nqr::PCMFormat::PCM_16)
		return AL_FORMAT_MONO16;
	else if (_channels == 2 && _format == nqr::PCMFormat::PCM_U8)
		return AL_FORMAT_STEREO8;
	else if (_channels == 2 && _format == nqr::PCMFormat::PCM_16)
		return AL_FORMAT_STEREO16;
	else
		return 0;
}

// ===========
// Music
// ===========
/*
void FRANAUDIO_API FranAudio::Music::Update(cl_entity_t* _ent)
{
	if (sampleDir == "ERR" || _ent == nullptr || sourceState == AL_STOPPED)
	{
		Kill();
		return;
	}

	if (firstUpdate)
	{
		firstUpdate = false;
	}

	FranAudio_AlFunction(alGetSourcei, sourceHandle, AL_SOURCE_STATE, &sourceState);

	//FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, _ent->origin);

	FranAudio_AlFunction(alSourcefv, sourceHandle, AL_POSITION, Vector(0,0,0));

	// if (_ent->model)
	// sampleHandle.setRadius(_ent->model->radius);
}

bool FranAudio::Music::PrecacheMusic(std::string _dir)
{
	if (SoundsMap.find(_dir) == SoundsMap.end())
	{
		FranAudio::Globals::LogMessage(std::string(std::string("Trying to load ") + "\"" + _dir.c_str() + "\"").c_str());

		SoundSample _sample{};

		if (SDL_LoadWAV(_dir.c_str(), &_sample.info, &_sample.data, &_sample.length) == NULL)
		{
			FranAudio::Globals::LogMessage(std::string(std::string("Error loading sound: ") + "\"" + _dir.c_str() + "\"").c_str());
			return false;
		}

		if (_channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_MONO8;
		else if (_channels == 1 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_MONO16;
		else if (_channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 8)
			_sample.format = AL_FORMAT_STEREO8;
		else if (_channels == 2 && SDL_AUDIO_BITSIZE(_sample.info.format) == 16)
			_sample.format = AL_FORMAT_STEREO16;

		SoundsMap.insert({_dir, _sample});
	}

	return true;
}

FranAudio::SoundSample FranAudio::Music::FindPrecachedMusic(std::string _dir)
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

*/
