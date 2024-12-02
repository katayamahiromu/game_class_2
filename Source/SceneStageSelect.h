#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <wrl.h>
#include "Audio/Audio.h"


//　タイトルセレクトシーン
class SceneStageSelect : public Scene
{
public:
	SceneStageSelect() {}
	~SceneStageSelect() override {}

	//　初期化
	void Initialize() override;

	//　終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	void DrawDebugGui();
private:
	//クラスの方がもしかしたらいいかも？
	//名前のほうもあんまりよくはないかもしれない	
	enum StageState
	{
		StageOne,
		StageTwo,
		StageMax,//ステージ数上限
	};

private:
	std::unique_ptr<Sprite> stage = nullptr;
	std::unique_ptr<Sprite> pin = nullptr;
	std::unique_ptr<Sprite> triangle = nullptr;
	std::unique_ptr<Sprite> line = nullptr;
	DirectX::XMFLOAT2 spriteSize = { 110.0f,100.0f };
	DirectX::XMFLOAT2 linePositions[10] = {
	{ 250.0f,300.0f },{450.0f,300.0f},{650.0f,300.0f},{850.0f,300.0f},{1050.0f,300.0f},
		{ 1250.0f,300.0f },{1450.0f,300.0f},{1650.0f,300.0f},{1850.0f,300.0f},{2050.0f,300.0f}
	};

	float scale[10] = { 1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };
	float angle[10] = { 45,-45,45,-45,45,-45,45,-45,45,-45 };
	float scrollScreenWidth = 0.0f;
	float charaMove = 0.0f;
	int selectNum = 0;
	int maxstage = 15;
	bool moveflg=false;
};