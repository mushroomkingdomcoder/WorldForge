#include "SoundSystem.h"
#include "Sound.h"

void STDMETHODCALLTYPE SoundSystem::Channel::VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	Channel& channel = *(Channel*)pBufferContext;
	channel.pSound->hasTerminated();
	channel.pSound = nullptr;
	SoundSystem::Get().DeactivateChannel(channel);
}

void SoundSystem::Channel::StartSound(Sound& sound, float freqMod, float volume)
{
	assert(pVoice && !pSound);
	if (!SoundFmtsAreEqual(curSndFmt, sound.format))
	{
		pVoice->DestroyVoice();
		pVoice = nullptr;
		SoundSystem::Get().pEngine->CreateSourceVoice(&pVoice, &sound.format, 0, 2.0f, &GetVCB());
		curSndFmt = sound.format;
	}
	sound.hasStarted(*this);
	pSound = &sound;
	buffer.pAudioData = sound.pSoundData.get();
	buffer.AudioBytes = sound.nSoundBytes;
	SNDCHECK(pVoice->SubmitSourceBuffer(&buffer, nullptr));
	SNDCHECK(pVoice->SetFrequencyRatio(freqMod));
	SNDCHECK(pVoice->SetVolume(volume));
	SNDCHECK(pVoice->Start());
}

SoundSystem::SoundSystem()
{
	SNDCHECK(CoInitialize(nullptr));
	SNDCHECK(XAudio2Create(&pEngine));
	SNDCHECK(pEngine->CreateMasteringVoice(&pMasterVoice));
	for (int i = 0; i < nChannels; ++i)
	{
		channels[i] = std::move(std::make_unique<Channel>(*this, i));
		channelIsActive[i] = false;
	}
	SetMasterVolume(1.0f);
}

void SoundSystem::DeactivateChannel(Channel& channel)
{
	channelIsActive[channel.GetID()] = false;
}

SoundSystem& SoundSystem::Get()
{
	static SoundSystem instance;
	return instance;
}

void SoundSystem::StartSound(Sound& sound, float freqMod, float volume)
{
	for (int i = 0; i < nChannels; ++i)
	{
		if (!channelIsActive[i])
		{
			channels[i]->StartSound(sound, freqMod, volume);
			channelIsActive[i] = true;
			break;
		}
	}
}

void SoundSystem::SetMasterVolume(float volume)
{
	SNDCHECK(pMasterVoice->SetVolume(volume));
}

float SoundSystem::GetMasterVolume() const
{
	float volume;
	pMasterVoice->GetVolume(&volume);
	return volume;
}

SoundSystem::~SoundSystem()
{
	pMasterVoice->DestroyVoice();
	pMasterVoice = nullptr;
	CoUninitialize();
}



