// FranticDreamer 2022
#pragma once

// DLL Exporting API Defines

#ifdef FRANAUDIO_DYNAMIC
#define FRANAUDIO_API __declspec(dllexport)
#else
#define FRANAUDIO_API __declspec(dllimport)
#endif
