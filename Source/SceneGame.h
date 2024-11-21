#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"

// ゲームシーン
class SceneGame:public Scene
{
public:
	//生成時にどのステージか決める
	SceneGame(int StageNum) :select(StageNum) {};
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
private:
	//エネミーHPゲージ描画
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

	int select;
};
