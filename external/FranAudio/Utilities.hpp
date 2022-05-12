#pragma once

#include "FranAudio.hpp"

#include <string>
#include <filesystem>

namespace FranAudio::Utilities
{
	// Replaces all occurences of a substring
	std::string ReplaceSubString(std::string subject, const std::string& search,
		const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	// Check if a file exists in mod dir or fallback dir.
	inline std::string ReturnPlayableDir(std::string file)
	{
		std::string temp;

		// Fix old paths like "barney/ambush"
		file = ReplaceSubString(file, "/", "\\");

		if (std::filesystem::exists(temp = (FranAudio::Globals::GetModSoundDir() + file)))
			return temp; // Mod dir file
		else if (std::filesystem::exists(temp = (FranAudio::Globals::GetFallbackSoundDir() + file)))
			return temp; // Fallback dir file
		else 
			return "ERR";
	}
};
