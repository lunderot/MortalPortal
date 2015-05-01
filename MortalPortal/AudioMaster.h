#pragma once

#include "Audio.h"
#include "AudioHandler.h"

#include <vector>
#include <map>

class AudioMaster
{
private:
	AudioHandler* audioHandler;
	std::vector<Audio*> audioSamples;
	std::map<std::string, unsigned int> audioMap;
protected:
public:
	AudioMaster();
	~AudioMaster();

	unsigned int addNewSample(TCHAR file[], std::string name, bool loop);
	void playSample(unsigned int index);
	void playSample(std::string name);
	void stopSample(unsigned int index);
	void stopSample(std::string name);
};