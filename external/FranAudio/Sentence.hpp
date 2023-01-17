// FranticDreamer 2022
#pragma once

#include <vector>

#include "FranAudioAPI.hpp"
#include "FranAudio.hpp"
#include "FranVector.hpp"

#include "Sound.hpp"

namespace FranAudio
{
	enum class SentenceFragmentModifierType : short
	{
		NONE = -1,
		Pitch = 0, // Changes the pitch. It also speeds up the sound by the same amount.
		Start,	   // Changes the starting position. Allows you to truncate the beginning of the file.
		End,	   // Changes the ending position. Allows you to truncate the end of the file.
		Volume,	   // Changes the volume.
		Time,	   // Time compression. It can only speed up, and starts from 0 (normal speed). Doesn't affect pitch. See below.
	};
	
	struct SentenceFragmentModifier
	{
		SentenceFragmentModifierType type;
		int data;

		SentenceFragmentModifier(SentenceFragmentModifierType _type, int _data);
	};

	struct SentenceFragment
	{
		std::string fragmentName;
		std::vector<SentenceFragmentModifier> modifiers;
		
		SentenceFragment(std::string _fragmentName, std::vector<SentenceFragmentModifier> _modifiers);
	};

	class Sentence
	{	
	protected:
		std::string baseDir;
		std::vector<SentenceFragment> fragments;

	public:
		Sentence();
		Sentence(std::string _baseDir, std::vector<SentenceFragment> _fragments);
	};

	class SentenceGroupManager
	{
	protected:
		static std::map<std::string, std::vector<Sentence>> sentenceGroups;

	public:
		Sentence GetSentence(const std::string& sentenceName);
		Sentence GetSentence(const std::string& sentenceGroupName, int sentenceIndex);
		
		Sentence GetRandomSentence(const std::string& sentenceGroupName);

	};








};