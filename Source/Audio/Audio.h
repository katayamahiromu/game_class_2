#pragma once

#include <xaudio2.h>
#include "Audio/AudioSource.h"
#include"Audio/SubMixSource.h"

// �I�[�f�B�I
class Audio
{
public:
	Audio();
	~Audio();

public:
	// �C���X�^���X�擾
	static Audio& Instance() { return *instance; }

	// �I�[�f�B�I�\�[�X�ǂݍ���
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename, IXAudio2SubmixVoice* sv = nullptr);
	//�T�u�~�b�N�X�{�C�X�̍쐬
	std::unique_ptr<SubMixVoice> MakeSubMix();
public:
	IXAudio2* Get_IXAudio2() { return xaudio; }
private:
	static Audio*			instance;

	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
