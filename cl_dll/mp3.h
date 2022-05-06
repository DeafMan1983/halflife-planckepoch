// OpenAL based MP3 System
// Some parts are based on Killar's code

#pragma once

#include "Platform.h"
#include "PlatformHeaders.h"

class CMP3
{
private:
	bool m_iIsPlaying;


public:
	int Initialize();
	int Shutdown();
	int PlayMP3(const char* pszSong);
	int StopMP3();
};

extern CMP3 gMP3;
