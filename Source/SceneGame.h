#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include"ScenePause.h"
#include"StageObject.h"
#include"Graphics/MaskShader.h"
#include"Audio/Audio.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	//�������ɂǂ̃X�e�[�W�����߂�
	SceneGame(int StageNum) :select(StageNum) {};
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	//�|�[�Y
	void Pause();

	//�|�[�Y�����
	void ClosePauseCheck();

	//�I�u�W�F�N�g�̕`�揈��
	void ObjectRender();

	//�I�u�W�F�N�g�̍Đݒ�
	void GameResetting();

	//���Z�b�g
	void Reset(float elapsedTime);

	//�N���A����
	void GameClear(float elapsedTime);

	//GUI
	void DebugGui();

	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

private:
	std::unique_ptr<Player> player;
	std::unique_ptr<CameraController>cameraController;
	std::unique_ptr<Sprite>gauge;
	std::unique_ptr<Sprite>back;

	std::unique_ptr<Sprite>UIMain;
	std::unique_ptr<Sprite>UIRec;
	std::unique_ptr<Sprite>UIRecording;
	std::unique_ptr<Sprite>UIPlay;
	std::unique_ptr<Sprite>UIStop;
	std::unique_ptr<Sprite>UISideNow;
	std::unique_ptr<Sprite>UITopNow;

	std::unique_ptr<Sprite>gameStartSprite;


	//�|�[�Y�̎��Ɍ��ɏo���Q�[�����
	std::unique_ptr<Sprite>gameScene;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>scene_render;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>scene_shader_resource_view;

	//�V�[���Q�[���Ƀ|�[�Y������̂͋����Ă��������ҍ�
	std::unique_ptr<ScenePause> pause;
	bool PauseFlag = false;

	//���s����
	DirectX::XMFLOAT4 ambientLightColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 lightDirection = { -1.0f, -1.0f, -1.0f, 0.0f };

	int select;

	//�}�X�N�p�̂��ꂱ��
	std::unique_ptr<Sprite>effectSprite;
	std::unique_ptr<Sprite>mask;
	std::unique_ptr<MaskShader>MS;

	//���Z�b�g�p
	float dissolveThreshold = 1.0f;
	float edgThreshold = 0.2f; //臒l
	DirectX::XMFLOAT4 edgColor = {1,0,0,1}; //�F

	//Debug�p
	std::vector<Cube*>cubeArray;

	//���Z�b�g�֌W
	bool isReset = false;
	float time = 0.0f;
	const float MAX_RESET_TIME = 0.2f;

	//���񂾂���1.0����n�߂��
	bool isFirstSetting = true;

	//���y
	std::unique_ptr<AudioSource>BGM;

	//�N���A����
	bool isClear = false;
	float clearTime = 0.0f;
	std::unique_ptr<Sprite>clearSprite;
	std::unique_ptr<AudioSource>ClearBGM;
};
