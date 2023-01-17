// FranticDreamer 2022

#include "PlatformHeaders.h"
#include "FranAudio.hpp"
#include "Channel.hpp"
#include "Sentence.hpp"

FranAudio::SentenceFragmentModifier::SentenceFragmentModifier(SentenceFragmentModifierType _type, int _data)
	: type(_type), data(_data)
{
}

FranAudio::SentenceFragment::SentenceFragment(std::string _fragmentName, std::vector<SentenceFragmentModifier> _modifiers)
	: fragmentName(_fragmentName), modifiers(_modifiers)
{
}

FranAudio::Sentence::Sentence()
{
}

FranAudio::Sentence::Sentence(std::string _baseDir, std::vector<SentenceFragment> _fragments)
	: baseDir(_baseDir), fragments(_fragments)
{
}
