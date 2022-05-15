// FranticDreamer 2022

#include "FranAudio.hpp"

#include "ALWrappers.hpp"

bool FranAudio::PrintALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		FranAudio::Globals::LogMessage(std::string("ERR: ( " + filename + ": " + std::to_string(line) + ")").c_str());
		switch (error)
		{
		case ALC_INVALID_VALUE:
			FranAudio::Globals::LogMessage("ERR: ALC_INVALID_VALUE!");
			break;
		case ALC_INVALID_DEVICE:
			FranAudio::Globals::LogMessage("ERR: ALC_INVALID_DEVICE!");
			break;
		case ALC_INVALID_CONTEXT:
			FranAudio::Globals::LogMessage("ERR: ALC_INVALID_CONTEXT!");
			break;
		case ALC_INVALID_ENUM:
			FranAudio::Globals::LogMessage("ERR: ALC_INVALID_ENUM!");
			break;
		case ALC_OUT_OF_MEMORY:
			FranAudio::Globals::LogMessage("ERR: ALC_OUT_OF_MEMORY!");
			break;
		default:
			FranAudio::Globals::LogMessage(std::string("ERR: UNKNOWN ALC ERROR! " + std::to_string(error)).c_str());
		}
		return false;
	}
	return true;
}

bool FranAudio::PrintALErrors(const std::string& filename, const std::uint_fast32_t line)
{
	ALenum error = alGetError();
	if (error != ALC_NO_ERROR)
	{
		FranAudio::Globals::LogMessage(std::string("ERR: ( " + filename + ": " + std::to_string(line) + ")").c_str());
		switch (error)
		{
		case AL_INVALID_NAME:
			FranAudio::Globals::LogMessage("ERR: AL_INVALID_NAME!");
			break;
		case AL_INVALID_ENUM:
			FranAudio::Globals::LogMessage("ERR: AL_INVALID_ENUM!");
			break;
		case AL_INVALID_VALUE:
			FranAudio::Globals::LogMessage("ERR: AL_INVALID_VALUE!");
			break;
		case AL_INVALID_OPERATION:
			FranAudio::Globals::LogMessage("ERR: AL_INVALID_OPERATION!");
			break;
		case AL_OUT_OF_MEMORY:
			FranAudio::Globals::LogMessage("ERR: AL_OUT_OF_MEMORY!");
			break;
		default:
			FranAudio::Globals::LogMessage(std::string("ERR: UNKNOWN AL ERROR: " + std::to_string(error)).c_str());
		}
		return false;
	}
	return true;
}