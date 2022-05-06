// OpenAL based MP3 System
// Some parts are based on Killar's code

#include "hud.h"
#include "cl_util.h"
#include "mp3.h"

int CMP3::Initialize()
{
	m_iIsPlaying = false;
	return 0;
}

int CMP3::Shutdown()
{
	m_iIsPlaying = false;
	return 0;
}

int CMP3::StopMP3( )
{
	m_iIsPlaying = false;
	return 0;
}

int CMP3::PlayMP3(const char* pszSong)
{
	m_iIsPlaying = true;
	return 0;
}