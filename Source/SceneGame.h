#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"

// �Q�[���V�[��
class SceneGame:public Scene
{
public:
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
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
private:
	Stage*  stage  = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	Sprite* gauge = nullptr;
	Sprite* back = nullptr;
};