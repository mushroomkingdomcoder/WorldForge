#include "Sound.h"
#include <fstream>

void Sound::hasStarted(SoundSystem::Channel& channel)
{
	pChannels.push_back({ &channel,true });
}

void Sound::hasTerminated()
{
	pChannels.front() = { nullptr,false };
	pChannels.erase(pChannels.begin());
}

Sound::Sound(const char* fileName, float freqMod, float volume)
	:
	freqMod(freqMod),
	volume(volume)
{
	std::ifstream wavIN{ fileName,std::ios::binary };
	int riff = 0;
	wavIN.read(reinterpret_cast<char*>(&riff), 4);
	if (riff != 'FFIR')
	{
		throw SNDEXCPT("RIFF .wav sound files only!\n" + std::string(fileName));
	}
	wavIN.seekg(4, std::ios::cur);
	int wave = 0;
	wavIN.read(reinterpret_cast<char*>(&wave), 4);
	if (wave != 'EVAW')
	{
		throw SNDEXCPT("Sound file must be of type .wav!\n" + std::string(fileName));
	}
	int fmt = 0;
	wavIN.read(reinterpret_cast<char*>(&fmt), 4);
	if (fmt != ' tmf')
	{
		throw SNDEXCPT("Format chunk of .wav file not found!\n" + std::string(fileName));
	}
	int fmtSize = 0;
	wavIN.read(reinterpret_cast<char*>(&fmtSize), 4);
	assert(fmtSize == 16);
	format.cbSize = 0;
	wavIN.read(reinterpret_cast<char*>(&format), fmtSize);
	int data = 0;
	bool foundData = false;
	while (!foundData)
	{
		char d = 0;
		while (d != 'd')
		{
			wavIN.read(&d, 1);
			if (wavIN.eof())
			{
				throw SNDEXCPT("Data chunk of .wav file not found!\n" + std::string(fileName));
				break;
			}
		}
		wavIN.seekg(-1, std::ios::cur);
		wavIN.read(reinterpret_cast<char*>(&data), 4);
		foundData = (data == 'atad');
		if (!foundData)
		{
			wavIN.seekg(-3, std::ios::cur);
		}
	}
	wavIN.read(reinterpret_cast<char*>(&nSoundBytes), 4);
	pSoundData = std::make_unique<BYTE[]>(nSoundBytes);
	wavIN.read(reinterpret_cast<char*>(pSoundData.get()), nSoundBytes);
}

Sound::Sound(Sound&& sound) noexcept
	:
	format(sound.format),
	nSoundBytes(sound.nSoundBytes),
	pSoundData(std::move(sound.pSoundData)),
	pChannels(std::move(sound.pChannels)),
	freqMod(sound.freqMod),
	volume(sound.volume)
{}

void Sound::Start()
{
	SoundSystem::Get().StartSound(*this, freqMod, volume);
}

void Sound::Stop()
{
	pChannels.front().first->StopSound();
}

void Sound::Resume(int instance)
{
	assert(instance < pChannels.size());
	assert(!pChannels[instance].second && "Sound was already playing");
	pChannels[instance].first->ResumeSound();
	pChannels[instance].second = true;
}

void Sound::Pause(int instance)
{
	assert(instance < pChannels.size());
	assert(pChannels[instance].second && "Sound was already paused");
	pChannels[instance].first->PauseSound();
	pChannels[instance].second = false;
}

void Sound::StopAll()
{
	while (isPlaying())
	{
		Stop();
	}
}

void Sound::PauseAll()
{
	assert(!pChannels.empty());
	for (std::pair<SoundSystem::Channel*, bool>& pChannel : pChannels)
	{
		if (pChannel.second)
		{
			pChannel.first->PauseSound();
			pChannel.second = false;
		}
	}
}

void Sound::ResumeAll()
{
	assert(!pChannels.empty());
	for (std::pair<SoundSystem::Channel*, bool>& pChannel : pChannels)
	{
		if (!pChannel.second)
		{
			pChannel.first->ResumeSound();
			pChannel.second = true;
		}
	}
}

void Sound::SetVolume(float volume)
{
	this->volume = volume;
	for (std::pair<SoundSystem::Channel*, bool>& pChannel : pChannels)
	{
		pChannel.first->SetVolume(volume);
	}
}

const float& Sound::GetVolume() const
{
	return volume;
}

const bool& Sound::isActivelyPlaying(int instance) const
{
	assert(instance < pChannels.size());
	return pChannels[instance].second;
}

bool Sound::isActivelyPlayingAny() const
{
	assert(!pChannels.empty());
	for (const std::pair<SoundSystem::Channel*, bool>& pChannel : pChannels)
	{
		if (pChannel.second)
		{
			return true;
		}
	}
	return false;
}

bool Sound::isPlaying() const
{
	return !pChannels.empty();
}

Sound::~Sound()
{
	StopAll();
}

