// FranticDreamer 2022
#pragma once

#include <vector>

#include "FranAudioAPI.hpp"
#include "FranAudio.hpp"
#include "FranVector.hpp"

#include "Sound.hpp"

namespace FranAudio
{
	enum class FRANAUDIO_API ChannelType : unsigned int
	{
		Auto = CHAN_AUTO,
		Weapon = CHAN_WEAPON,
		Voice = CHAN_VOICE,
		Item = CHAN_ITEM,
		Body = CHAN_BODY,
		Stream = CHAN_STREAM,
		Static = CHAN_STATIC,
		NetworkVoiceBase = CHAN_NETWORKVOICE_BASE,
		NetworkVoiceEnd = CHAN_NETWORKVOICE_END,
		Bot = CHAN_BOT,
	};
};
