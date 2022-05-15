// FranticDreamer 2022
#pragma once

#include <string>

namespace FranAudio::Utilities
{
	// Replaces all occurences of a substring
	std::string ReplaceSubString(std::string subject, const std::string& search,
	const std::string& replace);

	// Check if a file exists in mod dir or fallback dir.
	std::string ReturnPlayableDir(std::string file);
};
