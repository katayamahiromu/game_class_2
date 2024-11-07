#include "Misc.h"
#include "Audio/AudioSource.h"
#include"Audio/Audio.h"

// �R���X�g���N�^
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, IXAudio2SubmixVoice* sv)
	: resource(resource)
{
	HRESULT hr;

	// �\�[�X�{�C�X�𐶐�
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat(),XAUDIO2_VOICE_USEFILTER);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	if (sv!=nullptr)
	{
		//�T�u�~�b�N�X�{�C�X�ɓn��
		Set_Submix_voice(sv);
	}
}

// �f�X�g���N�^
AudioSource::~AudioSource()
{
	if (sourceVoice != nullptr)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = nullptr;
	}
}

// �Đ�
void AudioSource::Play(bool loop)
{
	Stop();
	// �\�[�X�{�C�X�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	
	sourceVoice->SubmitSourceBuffer(&buffer);

	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void AudioSource::DC_Play()
{
	Stop();
	sourceVoice->FlushSourceBuffers();
	// �\�[�X�{�C�X�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	sourceVoice->SubmitSourceBuffer(&buffer);

	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void AudioSource::Middle_Play(float time)
{
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = 0;
	buffer.PlayBegin = resource->GetWaveFormat().nSamplesPerSec * time;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	sourceVoice->SubmitSourceBuffer(&buffer);
	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void AudioSource::Sustain_Loop(float loop_start, float loop_end)
{
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopBegin = resource->GetWaveFormat().nSamplesPerSec * loop_start;
	buffer.LoopLength = resource->GetWaveFormat().nSamplesPerSec * loop_end;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	sourceVoice->SubmitSourceBuffer(&buffer);
	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// ��~
void AudioSource::Stop()
{
	sourceVoice->Stop();
}

//�{�����[��
void AudioSource::Set_Volume(float volum)
{
	volum_ = volum;
	sourceVoice->SetVolume(volum_);
}

//�f�V�x����ݒ�
float AudioSource::Set_Decibel(float decibls)
{
	return XAudio2AmplitudeRatioToDecibels(decibls);
}


void  AudioSource::Pitch_Shift(float pitch)
{
	//���[�N���N���邽�ߌ���̃\�[�X�{�C�X��j��
	sourceVoice->DestroyVoice();
	
	//WAVE�̃t�H�[�}�b�g�̏�������

	//�V�����\�[�X�{�C�X�̍쐬
	HRESULT hr;
	hr = Audio::Instance().Get_IXAudio2()->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat(), XAUDIO2_VOICE_USEFILTER);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//�s�b�`�̕ύX
	sourceVoice->SetFrequencyRatio(1 / pitch);
}

//�s�b�`
void AudioSource::Set_Pitch(float Pitch)
{
	sourceVoice->SetFrequencyRatio(Pitch);
}

//�s�b�`�̃��Z�b�g
void AudioSource::Reset_Pitch()
{
	sourceVoice->SetFrequencyRatio(1.0f);
}

//�|�[�Y
void AudioSource::Pause()
{
	sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
}

//���X�^�[�g
void AudioSource::ReStart()
{
	sourceVoice->Start();
}

//�t�B���^�[
void AudioSource::Filter(int type)
{
	XAUDIO2_FILTER_PARAMETERS paramers{};

	switch (type)
	{
	case Fliter_Type::LOW_PASS_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
		paramers.Frequency = 0.5f / resource->GetWaveFormat().nSamplesPerSec * 6.0f;
		paramers.OneOverQ = 1.4142f;
		break;
	case Fliter_Type::HIGH_PASS_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::HighPassFilter;
		paramers.Frequency = 0.5f / resource->GetWaveFormat().nSamplesPerSec * 6.0f;
		paramers.OneOverQ = 1.4142f;
		break;
	case Fliter_Type::BAND_PASS_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
		paramers.Frequency = 0.5f;
		paramers.OneOverQ = 1.0f;
		break;
	case Fliter_Type::NOTCH_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::NotchFilter;
		paramers.Frequency = 0.5f;
		paramers.OneOverQ = 1.0f;
		break;
	case Fliter_Type::LOW_PASS_ONE_POLE_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::LowPassOnePoleFilter;
		paramers.Frequency = XAudio2CutoffFrequencyToOnePoleCoefficient(0.5f, resource->GetWaveFormat().nSamplesPerSec);
		break;
	case Fliter_Type::HIGH_PASS_ONE_POLE_FILTER:
		paramers.Type = XAUDIO2_FILTER_TYPE::HighPassOnePoleFilter;
		paramers.Frequency = XAudio2CutoffFrequencyToOnePoleCoefficient(0.5f, resource->GetWaveFormat().nSamplesPerSec);
		break;
	}


	sourceVoice->SetFilterParameters(&paramers);
}


bool AudioSource::IsPlay()
{
	XAUDIO2_VOICE_STATE state;
	sourceVoice->GetState(&state);
	//BuffersQueued �� 0 �łȂ��A�܂��� SamplesPlayed �� 0 �łȂ��ꍇ�A�Đ����Ƃ݂Ȃ�
	if (state.BuffersQueued > 0 || state.SamplesPlayed > 0)return true;
	return false;
}

void AudioSource::Set_Submix_voice(IXAudio2SubmixVoice* sv)
{
	HRESULT hr;
	//�T�u�~�b�N�X�{�C�X�ɓn��
	XAUDIO2_SEND_DESCRIPTOR send = { 0,sv };
	XAUDIO2_VOICE_SENDS sendlist = { 1,&send };
	hr = sourceVoice->SetOutputVoices(&sendlist);
}