#pragma once
#include "SoundSystem.h"
#include <assert.h>

class Sound
{
	friend SoundSystem;
private:
	WAVEFORMATEX format;
	UINT32 nSoundBytes;
	std::unique_ptr<BYTE[]> pSoundData;
	std::vector<std::pair<SoundSystem::Channel*, bool>> pChannels;
	float freqMod;
	float volume;
private:
	void hasStarted(SoundSystem::Channel& channel);
	void hasTerminated();
public:
	Sound() = delete;
	Sound(const char* fileName, float freqMod = 1.0f, float volume = 1.0f);
	Sound(const Sound& sound) = delete;
	Sound operator =(const Sound& sound) = delete;
	Sound(Sound&& sound) noexcept;
	void Start();
	void Stop();
	void Resume(int instance);
	void Pause(int instance);
	void StopAll();
	void PauseAll();
	void ResumeAll();
	void SetVolume(float volume);
	const float& GetVolume() const;
	const bool& isActivelyPlaying(int instance) const;
	bool isActivelyPlayingAny() const;
	bool isPlaying() const;
	~Sound();
};


