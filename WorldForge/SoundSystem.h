#pragma once
#include "BaseException.h"
#include <xaudio2.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include <bitset>
#include <assert.h>
#include <optional>
#pragma comment(lib, "xaudio2.lib")

class SoundSystem
{
public:
	static constexpr int nChannels = 64;
public:
	class Exception : public BaseException
	{
	private:
		std::optional<HRESULT> hr;
	private:
		std::string GetErrorCodeString() const noexcept
		{
			if (!hr)
			{
				return "N/A";
			}
			else
			{
				std::ostringstream err;
				err << *hr;
				return err.str();
			}
		}
		std::string GetDescriptionString() const noexcept
		{
			if (!hr)
			{
				return "N/A";
			}
			else
			{
				return GetErrorCodeDesc(*hr);
			}
		}
	public:
		Exception() = delete;
		Exception(int line, std::string file, std::string note)
			:
			BaseException(line, file, note),
			hr()
		{}
		Exception(int line, std::string file, HRESULT hr, std::string note = "") noexcept
			:
			BaseException(line, file, note),
			hr(hr)
		{}
		const char* what() const noexcept override
		{
			std::ostringstream wht;
			wht << "Exception Type: " << GetType() << std::endl
				<< "Error Code: " << GetErrorCodeString() << std::endl
				<< "Description: " << GetDescriptionString() << std::endl
				<< "File Name: " << GetFile() << std::endl
				<< "Line Number: " << GetLine() << std::endl
				<< "Additional Info: " << GetNote() << std::endl;
			whatBuffer = wht.str();
			return whatBuffer.c_str();
		}
		const char* GetType() const noexcept override
		{
			return "FantasyForge XAudio2 Exception";
		}
	};
	#define SNDEXCPT(hr_or_note) SoundSystem::Exception(__LINE__, __FILE__, hr_or_note)
	#define SNDEXCPT_NOTE(hr, note) SoundSystem::Exception(__LINE__, __FILE__, hr, note)
	#define SNDCHECK(hr) if (FAILED(hr)) { throw SNDEXCPT(hr); }
	class Channel
	{
		friend SoundSystem;
	private:
		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			void STDMETHODCALLTYPE OnStreamEnd() override {}
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {}
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samplesRequired) override {}
			void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override {}
			void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override {}
			void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT error) override {}
		};
	private:
		WAVEFORMATEX curSndFmt;
		XAUDIO2_BUFFER buffer;
		IXAudio2SourceVoice* pVoice;
		class Sound* pSound;
		int id;
	private:
		void StartSound(class Sound& sound, float freqMod, float volume);
	public:
		Channel() = delete;
		Channel(SoundSystem& soundSys, int id)
			:
			id(id)
		{
			curSndFmt.wFormatTag = WAVE_FORMAT_PCM;
			curSndFmt.nChannels = 2;
			curSndFmt.nSamplesPerSec = 44100;
			curSndFmt.nAvgBytesPerSec = 176400;
			curSndFmt.nBlockAlign = 4;
			curSndFmt.wBitsPerSample = 16;
			curSndFmt.cbSize = 0;
			static VoiceCallback vcb;
			ZeroMemory(&buffer, sizeof(buffer));
			buffer.pContext = this;
			SNDCHECK(soundSys.pEngine->CreateSourceVoice(&pVoice, &curSndFmt, 0, 2.0f, &GetVCB()));
		}
		Channel(const Channel& channel) = delete;
		Channel operator =(const Channel& channel) = delete;
		void StopSound()
		{
			assert(pVoice && pSound);
			SNDCHECK(pVoice->Stop());
			SNDCHECK(pVoice->FlushSourceBuffers());
			while (pSound);
		}
		void ResumeSound()
		{
			assert(pVoice && pSound);
			SNDCHECK(pVoice->Start());
		}
		void PauseSound()
		{
			assert(pVoice && pSound);
			SNDCHECK(pVoice->Stop());
		}
		void SetVolume(float volume)
		{
			assert(pVoice && pSound);
			SNDCHECK(pVoice->SetVolume(volume));
		}
		const int& GetID() const
		{
			return id;
		}
		class Sound* GetSoundPtr()
		{
			return pSound;
		}
		~Channel()
		{
			assert(!pSound);
			if (pVoice)
			{
				pVoice->DestroyVoice();
				pVoice = nullptr;
			}
		}
	private:
		static VoiceCallback& GetVCB()
		{
			static VoiceCallback vcb;
			return vcb;
		}
		static bool SoundFmtsAreEqual(WAVEFORMATEX left, WAVEFORMATEX right)
		{
			return (
				left.wFormatTag			==		right.wFormatTag		&&
				left.nChannels			==		right.nChannels			&&
				left.nSamplesPerSec		==		right.nSamplesPerSec	&&
				left.nAvgBytesPerSec	==		right.nAvgBytesPerSec	&&
				left.nBlockAlign		==		right.nBlockAlign		&&
				left.wBitsPerSample		==		right.wBitsPerSample	);
		}
	};
private:
	Microsoft::WRL::ComPtr<IXAudio2> pEngine;
	IXAudio2MasteringVoice* pMasterVoice;
	std::unique_ptr<Channel> channels[nChannels];
	std::bitset<nChannels> channelIsActive;
private:
	SoundSystem();
	void DeactivateChannel(Channel& channel);
public:
	SoundSystem(const SoundSystem& soundSys) = delete;
	SoundSystem operator =(const SoundSystem& soundSys) = delete;
	static SoundSystem& Get();
	void StartSound(class Sound& sound, float freqMod, float volume);
	void SetMasterVolume(float volume);
	float GetMasterVolume() const;
	~SoundSystem();
};

