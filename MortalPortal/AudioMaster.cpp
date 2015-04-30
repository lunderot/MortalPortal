#include "AudioMaster.h"

AudioMaster::AudioMaster()
{
	this->audioHandler = new AudioHandler();
}

AudioMaster::~AudioMaster()
{

}

int AudioMaster::addNewSample(TCHAR file[])
{
	Audio* temp = new Audio(this->audioHandler, file);

	temp->loadAudio();

	this->audioSamples.push_back(temp);

	return this->audioSamples.size() - 1;
}

void AudioMaster::playSample(int index)
{
	this->audioSamples[index]->playAudio();
}