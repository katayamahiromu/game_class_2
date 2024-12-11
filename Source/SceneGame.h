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

	//オブジェクトの再設定
	void GameResetting();

	//リセット
	void Reset(float elapsedTime);

	//クリア判定
	void GameClear(float elapsedTime);

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

	std::unique_ptr<Sprite>UIMain;
	std::unique_ptr<Sprite>UIRec;
	std::unique_ptr<Sprite>UIRecording;
	std::unique_ptr<Sprite>UIPlay;
	std::unique_ptr<Sprite>UIStop;
	std::unique_ptr<Sprite>UISideNow;
	std::unique_ptr<Sprite>UITopNow;

	std::unique_ptr<Sprite>gameStartSprite;


	//ポーズの時に後ろに出すゲーム画面
	std::unique_ptr<Sprite>gameScene;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>scene_render;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>scene_shader_resource_view;

	//シーンゲームにポーズがあるのは許してください辻合
	std::unique_ptr<ScenePause> pause;
	bool PauseFlag = false;

	//平行光源
	DirectX::XMFLOAT4 ambientLightColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 lightDirection = { -1.0f, -1.0f, -1.0f, 0.0f };

	int select;

	//マスク用のあれこれ
	std::unique_ptr<Sprite>effectSprite;
	std::unique_ptr<Sprite>mask;
	std::unique_ptr<MaskShader>MS;

	//リセット用
	float dissolveThreshold = 1.0f;
	float edgThreshold = 0.2f; //閾値
	DirectX::XMFLOAT4 edgColor = {1,0,0,1}; //色

	//Debug用
	std::vector<Cube*>cubeArray;

	//リセット関係
	bool isReset = false;
	float time = 0.0f;
	const float MAX_RESET_TIME = 0.2f;

	//初回だけは1.0から始める為
	bool isFirstSetting = true;

	//音楽
	std::unique_ptr<AudioSource>BGM;

	//クリア周り
	bool isClear = false;
	float clearTime = 0.0f;
	std::unique_ptr<Sprite>clearSprite;
	std::unique_ptr<AudioSource>ClearBGM;
};
