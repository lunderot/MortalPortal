#include "Audio.h"

Audio::Audio(AudioHandler* handle)
{
	this->Handle = handle;
	this->Source = nullptr;
	this->file = nullptr;

	this->buffer = { 0 };
	ZeroMemory(&this->wfx, sizeof(this->wfx));
}

Audio::Audio(AudioHandler* handle, TCHAR file[])
{
	this->Handle = handle;
	this->Source = nullptr;
	this->file = file;

	this->buffer = { 0 };
	ZeroMemory(&this->wfx, sizeof(this->wfx));
}

Audio::~Audio()
{
	delete[] this->pDataBuffer;
}

HRESULT Audio::loadAudio(bool loop)
{
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
}

HRESULT Audio::loadAudio(TCHAR file[], bool loop)
{
	this->file = file;

	return this->loadAudio(loop);
}

HRESULT Audio::playAudio()
{
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
}

void Audio::setFile(TCHAR file[])
{
	this->file = file;
}

TCHAR* Audio::getFile()
{
	return this->file;
}

void Audio::setHandler(AudioHandler* handle)
{
	this->Handle = handle;
}

HRESULT Audio::stopAudio()
{
	this->Source->Stop(0);

	return S_OK;
}
