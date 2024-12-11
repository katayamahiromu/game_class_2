#pragma once

#include"Graphics/Sprite.h"
#include"Audio/Audio.h"
#include<memory>

class ScenePause
{
public:
	ScenePause() {};
	~ScenePause(){};

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render();

	bool GetCloseFlag() { return closeFlag; }

	//閉じるフラグのリセット
	void ResetCloseFlag() { closeFlag = false; }

	void SetShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>Srv);
private:
	enum Command
	{
		CONTINNUE = 0,
		INSTRUCTION,
		STAGE_SELECTION,
	};
	void DebugGui();
private:
	float alpha = 0.4f;
	static const int MAX_COMMAND = 3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>gameSrv;
	std::unique_ptr<Sprite>back;
	std::unique_ptr<Sprite>oasobi;
	std::unique_ptr<Sprite>mask;
	std::unique_ptr<Sprite>command[MAX_COMMAND];
	bool closeFlag = false;
	bool chengSceneFlag = false;
	bool isExplain = false;

	int section = 0;
	float section_alpha = 0.5f;

	DirectX::XMFLOAT3 RGB = { 1.0f,1.0f,1.0f };


	//それぞれの画像の配置でーた
	// がっつり横着します
	//pos.x dx
	//pos.y dy
	//pos.z dw
	//pos.w dh
	DirectX::XMFLOAT4 pos[MAX_COMMAND] =
	{
		{45.0f, 0.0f, 1280.0f, 260.0f},
		{285.0f, 260.0f, 800.0f, 260.0f},
		{285.0f, 520.0f, 800.0f, 260.0f}
	};

	std::unique_ptr<AudioSource>select_note;
	std::unique_ptr<AudioSource>decide;


	std::unique_ptr<Sprite>ExplainSprite1;
	std::unique_ptr<Sprite>ExplainSprite2;
	int EXCount = 1;
};