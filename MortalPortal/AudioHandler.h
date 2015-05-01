#pragma once

// FOURCC endianess stuff
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

#include "DisableAudio.h"

#ifndef __DISABLE_AUDIO__
#include <xaudio2.h>
#else
#include "Bullshit.h"
#endif

#include <fstream>
#include <Windows.h>

class AudioHandler
{
public:
	AudioHandler();
	~AudioHandler();

	HRESULT findChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition);
	HRESULT readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT createSource(IXAudio2SourceVoice* &source, WAVEFORMATEXTENSIBLE &wfx);
protected:
private:
	IXAudio2* Engine;
	IXAudio2MasteringVoice* Master;

	HRESULT initialize();
};