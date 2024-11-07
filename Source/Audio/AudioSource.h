#pragma once

#include <memory>
#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "Audio/AudioResource.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, IXAudio2SubmixVoice* sv = nullptr);
	~AudioSource();

	// �Đ�
	void Play(bool loop);

	//�Ă΂��Ƃ܂��ŏ�����Đ�
	void DC_Play();

	//�r������Đ�
	void Middle_Play(float Time);

	//�w�肵�������烋�[�v
	void Sustain_Loop(float loop_start, float loop_end);

	// ��~
	void Stop();

	//�{�����[��
	void Set_Volume(float volum);

	//�f�V�x����ݒ�
	float Set_Decibel(float decibls);

	//�s�b�`
	void Set_Pitch(float Pitch);

	void Pitch_Shift(float pitch);

	//�s�b�`�̃��Z�b�g
	void Reset_Pitch();

	//�ꎞ��~
	void Pause();

	//���X�^�[�g
	void ReStart();

	//�t�B���^�[
	void Filter(int type);

	//�Đ�����
	bool IsPlay();

	//�T�u�~�b�N�X�{�C�X�֓n��
	void Set_Submix_voice(IXAudio2SubmixVoice* sv);
private:
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	float volum_ = 1.0;
public:
	enum Fliter_Type
	{
		LOW_PASS_FILTER,
		HIGH_PASS_FILTER,
		BAND_PASS_FILTER,
		NOTCH_FILTER,
		LOW_PASS_ONE_POLE_FILTER,
		HIGH_PASS_ONE_POLE_FILTER,
	};
};
