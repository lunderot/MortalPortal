#pragma once

#include <fstream>

#include "AudioHandler.h"

#ifndef __DISABLE_AUDIO__
#pragma comment (lib, "xaudio2.lib")
#else
#include "Bullshit.h"
#endif

class Audio
{
public:
	Audio(AudioHandler* handle);
	Audio(AudioHandler* handle, TCHAR file[]);
	~Audio();

	HRESULT loadAudio(bool loop);
	HRESULT loadAudio(TCHAR file[], bool loop);
	HRESULT playAudio();
	HRESULT stopAudio();
	
	void setFile(TCHAR file[]);
	TCHAR* getFile();
	void setHandler(AudioHandler* handle);
protected:
private:
	AudioHandler* Handle;
	IXAudio2SourceVoice* Source;
	TCHAR* file;

	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	BYTE* pDataBuffer;
};