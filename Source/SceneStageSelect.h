#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <wrl.h>
#include "Audio/Audio.h"


//�@�^�C�g���Z���N�g�V�[��
class SceneStageSelect : public Scene
{
public:
	SceneStageSelect() {}
	~SceneStageSelect() override {}

	//�@������
	void Initialize() override;

	//�@�I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	void DrawDebugGui();
private:
	//�N���X�̕��������������炢�������H
	//���O�̂ق�������܂�悭�͂Ȃ���������Ȃ�	
	enum StageState
	{
		StageOne,
		StageTwo,
		StageMax,//�X�e�[�W�����
	};

private:
	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite>  yajirusi = nullptr;

	DirectX::XMFLOAT2 selectPos = { 0,600 };

	int select = 0;

	int page = 0;

	int Gui;
};