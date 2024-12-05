#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include"ScenePause.h"

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
	//ポーズ
	void Pause();

	//ポーズを閉じる
	void ClosePauseCheck();

	//オブジェクトの描画処理
	void ObjectRender();

	void GameSetting();

	//GUI
	void DebugGui();

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

	//ポーズの時に後ろに出すゲーム画面
	std::unique_ptr<Sprite>gameScene;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>scene_render;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>scene_shader_resource_view;

	//シーンゲームにポーズがあるのは許してください辻合
	std::unique_ptr<ScenePause> pause;
	bool PauseFlag = false;

	//平行光源
	DirectX::XMFLOAT4 ambientLightColor = { 0.2f,0.2f,0.2f,1.0f };
	DirectX::XMFLOAT4 lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

	int select;
};
