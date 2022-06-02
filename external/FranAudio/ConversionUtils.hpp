// FranticDreamer 2022
#pragma once

#include "PlatformHeaders.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <map>

#include "FranAudioAPI.hpp"

namespace FranAudio::ConversionUtils
{
	void Resample_8to16(uint8_t* in, int16_t* out, size_t size);
	void Resample_32to8(float_t* in, uint8_t* out, size_t size);
};

