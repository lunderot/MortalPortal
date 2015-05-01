#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
#ifndef __DISABLE_AUDIO__
	this->Engine = nullptr;
	this->Master = nullptr;

	this->initialize();
#else
#endif
}

AudioHandler::~AudioHandler()
{
#ifndef __DISABLE_AUDIO__
	this->Engine->Release();
#else
#endif
}

HRESULT AudioHandler::initialize()
{
#ifndef __DISABLE_AUDIO__
	HRESULT hr;

	hr = XAudio2Create(&this->Engine, 0, XAUDIO2_DEFAULT_PROCESSOR);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create audio engine");
	}

	hr = this->Engine->CreateMasteringVoice(&this->Master);

	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create audio device");
	}

	return hr;
#else
	return S_OK;
#endif
}

HRESULT AudioHandler::findChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition)
{
#ifndef __DISABLE_AUDIO__
	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		//throw std::runtime_error("Invalid handle pointer for audio");
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;

		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
#else
return S_OK;
#endif
}

HRESULT AudioHandler::readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
#ifndef __DISABLE_AUDIO__
	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwRead;

	if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	return hr;
#else
	return S_OK;
#endif
}

HRESULT AudioHandler::createSource(IXAudio2SourceVoice* &source, WAVEFORMATEXTENSIBLE &wfx)
{
#ifndef __DISABLE_AUDIO__
	HRESULT hr;

	hr = this->Engine->CreateSourceVoice(&source, (WAVEFORMATEX*)&wfx);

	return hr;
#else
	return S_OK;
#endif
}
