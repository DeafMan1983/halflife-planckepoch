// FranticDreamer 2022

#include "ConversionUtils.hpp"

void FranAudio::ConversionUtils::Resample_8to16(uint8_t* in, int16_t* out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[i] = (int16_t)((in[i] - 128) << 8);
	}
}

void FranAudio::ConversionUtils::Resample_32to8(float_t* in, uint8_t* out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		// Dither?
		out[i] = (uint8_t)(128 + ((uint8_t)(out[i] * (127.0f))));
	}
}
