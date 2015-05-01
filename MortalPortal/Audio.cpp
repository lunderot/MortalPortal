#include "Audio.h"

Audio::Audio(AudioHandler* handle)
{
#ifndef __DISABLE_AUDIO__
	this->Handle = handle;
	this->Source = nullptr;
	this->file = nullptr;

	this->buffer = { 0 };
	ZeroMemory(&this->wfx, sizeof(this->wfx));
#else
#endif
}

Audio::Audio(AudioHandler* handle, TCHAR file[])
{
#ifndef __DISABLE_AUDIO__
	this->Handle = handle;
	this->Source = nullptr;
	this->file = file;

	this->buffer = { 0 };
	ZeroMemory(&this->wfx, sizeof(this->wfx));
#else
#endif
}

Audio::~Audio()
{
#ifndef __DISABLE_AUDIO__
	delete[] this->pDataBuffer;
#else
#endif
}

HRESULT Audio::loadAudio(bool loop)
{
#ifndef __DISABLE_AUDIO__
	HANDLE hFile = CreateFile(
		this->file,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	Handle->findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);

	DWORD filetype;

	Handle->readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
	{
		return S_FALSE;
	}

	Handle->findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	Handle->readChunkData(hFile, &this->wfx, dwChunkSize, dwChunkPosition);

	Handle->findChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	this->pDataBuffer = new BYTE[dwChunkSize];

	Handle->readChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	this->buffer.AudioBytes = dwChunkSize;
	this->buffer.pAudioData = pDataBuffer;
	this->buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loop == true)
	{
		this->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	return S_OK;
#else
	return S_OK;
#endif
}

HRESULT Audio::loadAudio(TCHAR file[], bool loop)
{
#ifndef __DISABLE_AUDIO__
	this->file = file;

	return this->loadAudio(loop);
#else
	return S_OK;
#endif
}

HRESULT Audio::playAudio()
{
#ifndef __DISABLE_AUDIO__
	HRESULT hr;

	hr = Handle->createSource(this->Source, this->wfx);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = this->Source->SubmitSourceBuffer(&this->buffer);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = Source->Start(0);

	return hr;
#else
	return S_OK;
#endif
}

void Audio::setFile(TCHAR file[])
{
#ifndef __DISABLE_AUDIO__
	this->file = file;
#else
#endif
}

TCHAR* Audio::getFile()
{
#ifndef __DISABLE_AUDIO__
	return this->file;
#else
	return this->file;
#endif
}

void Audio::setHandler(AudioHandler* handle)
{
#ifndef __DISABLE_AUDIO__
	this->Handle = handle;
#else
#endif
}

HRESULT Audio::stopAudio()
{
#ifndef __DISABLE_AUDIO__
	this->Source->Stop(0);

	return S_OK;
#else
	return S_OK;
#endif
}
