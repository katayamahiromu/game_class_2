#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include"ScenePause.h"

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
	void ClosePause();
	//�I�u�W�F�N�g�̕`�揈��
	void ObjectRender();

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

	//�|�[�Y�̎��Ɍ��ɏo���Q�[�����
	std::unique_ptr<Sprite>gameScene;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>scene_render;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>scene_shader_resource_view;

	ScenePause* pause = nullptr;
	bool PauseFlag = false;

	int select;
};
