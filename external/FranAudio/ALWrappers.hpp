#pragma once

#include "AL/al.h"
#include "AL/alc.h"

#define FranAudio_AlcFunction(function, device, ...) FranAudio::AlcFunctionImp(__FILE__, __LINE__, function, device, __VA_ARGS__)
#define FranAudio_AlFunction(function, ...) FranAudio::AlFunctionImp(__FILE__, __LINE__, function, __VA_ARGS__)

namespace FranAudio
{

bool PrintALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);
bool PrintALErrors(const std::string& filename, const std::uint_fast32_t line);


template <typename alFunction, typename... Params>
auto AlFunctionImp(const char* filename,
	const std::uint_fast32_t line,
	alFunction function,
	Params... params)
	-> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
	auto ret = function(std::forward<Params>(params)...);
	PrintALErrors(filename, line);
	return ret;
}

template <typename alFunction, typename... Params>
auto AlFunctionImp(const char* filename,
	const std::uint_fast32_t line,
	alFunction function,
	Params... params)
	-> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return PrintALErrors(filename, line);
}

template <typename alcFunction, typename... Params>
auto AlcFunctionImp(const char* filename,
	const std::uint_fast32_t line,
	alcFunction function,
	ALCdevice* device,
	Params... params) -> typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
	function(std::forward<Params>(params)...);
	return PrintALCErrors(filename, line, device);
}

template <typename alcFunction, typename ReturnType, typename... Params>
auto AlcFunctionImp(const char* filename,
	const std::uint_fast32_t line,
	alcFunction function,
	ReturnType& returnValue,
	ALCdevice* device,
	Params... params) -> typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
{
	returnValue = function(std::forward<Params>(params)...);
	return PrintALCErrors(filename, line, device);
}

};