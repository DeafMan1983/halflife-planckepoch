// FranticDreamer 2022

#include "FranAudio.hpp"
#include "Channel.hpp"
#include "Sound.hpp"

#include "Utilities.hpp"

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

	if (sampleData.load(sampleDir.c_str()) == SoLoud::SO_NO_ERROR)
	{
		// Load unsuccessful
		sampleDir = "ERR";
		return;
	}

	sampleData.setLooping(1);
	sampleData.set3dMinMaxDistance(1, 200);
	sampleData.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5);

	sampleHandle = FranAudio::Globals::GetContext().play3d(sampleData, 100, 0, 0);

	FranAudio::Globals::LogMessage(std::string("Sample Loaded. Length : ").c_str());

	//if (!audioSource)
	{
		Kill();
		return;
	}

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
	if (sampleDir == "ERR" || _ent == nullptr /* || !audioSource || audioSource.isPending()*/)
	{
	//	Kill();
		return;
	}

	FranAudio::SetSoundPosition(sampleHandle, _ent->origin);
	
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

void FranAudio::Sound::Kill()
{
	//if (audioSource)
	//	audioSource->Stop(); // Stop first

	//if (!sampleData && sampleDir != "ERR")
		//unloadsample(sampleData);

	sampleDir = "ERR";

	//for (auto& channel : FranAudio::Channel::channelsVec)
	//{
		// Remove this sound from the list;
	//	channel.sounds.PopIndex(channel.sounds.Find(*this));
	//}
}

int FRANAUDIO_API FranAudio::Sound::EntIndex()
{
	return entity;
}



