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
	bool ChangeFlg(float elapsedTime);

	void ShiftChara(float elapsedTime);

	void ChangeSize(float elapsedTime);
private:
	std::unique_ptr<Sprite> stage = nullptr;
	std::unique_ptr<Sprite> pin = nullptr;
	std::unique_ptr<Sprite> triangle = nullptr;
	std::unique_ptr<Sprite> line = nullptr;
	std::unique_ptr<Sprite> stageNum[20] = {};
	DirectX::XMFLOAT2 spriteSize = { 110.0f,100.0f };

	float scale[20] = { 1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
					 1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };
	float scrollScreenWidth = 0.0f;
	float charaMove = 0.0f;
	int selectNum = 0;
	int maxStage = 20;
	bool moveFlg = false;
	bool cooltimeFlg = false;
	float cooltime = 0.0f;

	int select = 0;

	int page = 0;

	int Gui;

	int debug_int = 0;

	std::unique_ptr<AudioSource>BGM;
	std::unique_ptr<AudioSource>select_note;
	std::unique_ptr<AudioSource>decide;
	bool chengeFlag = false;
};