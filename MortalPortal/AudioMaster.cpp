#include "AudioMaster.h"

AudioMaster::AudioMaster()
{
	this->audioHandler = new AudioHandler();
}

AudioMaster::~AudioMaster()
{
	unsigned int temp = this->audioSamples.size();

	delete this->audioHandler;

	for (unsigned int i = 0; i < temp; i++)
	{
		delete this->audioSamples[i];
	}
}

unsigned int AudioMaster::addNewSample(TCHAR file[], std::string name, bool loop)
{
	Audio* temp = new Audio(this->audioHandler, file);

	temp->loadAudio(loop);

	this->audioSamples.push_back(temp);

	this->audioMap[name] = this->audioSamples.size() - 1;

	return this->audioSamples.size() - 1;
}

void AudioMaster::playSample(unsigned int index)
{
	this->audioSamples[index]->playAudio();
}

void AudioMaster::playSample(std::string name)
{
	auto it = this->audioMap.find(name);
	if (it != this->audioMap.end())
	{
		this->audioSamples[it->second]->playAudio();
	}
}

void AudioMaster::stopSample(unsigned int index)
{
	this->audioSamples[index]->stopAudio();
}

void AudioMaster::stopSample(std::string name)
{
	auto it = this->audioMap.find(name);
	if (it != this->audioMap.end())
	{
		this->audioSamples[it->second]->stopAudio();
	}
}
