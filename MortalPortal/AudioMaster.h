#pragma once

#include "Audio.h"
#include "AudioHandler.h"

#include <vector>

class AudioMaster
{
private:
	AudioHandler* audioHandler;
	std::vector<Audio*> audioSamples;
protected:
public:
	AudioMaster();
	~AudioMaster();

	int addNewSample(TCHAR file[]);
	void playSample(int index);
};