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
	//追加
	std::unique_ptr<Sprite> stage = nullptr;
	std::unique_ptr<Sprite> pin = nullptr;
	DirectX::XMFLOAT2 spriteSize = { 110.0f,100.0f };
	//フルスクリーン(1920,1080)の座標
	/*DirectX::XMFLOAT2 pinPositions[10] = { {200.0f,300.0f},{550,700.0f},{900.0f,300.0f},{1250.0f,700.0f},{1600.0f,300.0f},
							{200.0f,700.0f},{550.0f,300.0f},{900.0f,700.0f},{1250.0f,300.0f},{1600.0f,700.0f} };
	DirectX::XMFLOAT2 stagePositions[10] = { {150.0f, 400.0f}, {500.0f, 800.0f}, {850.0f, 400.0f},{1200.0f, 800.0f}, {1550.0f, 400.0f},
		{1950.0f, 800.0f},{2300.0f, 400.0f}, {2650.0f, 800.0f}, {3000.0f, 400.0f},{3350.0f, 800.0f} };*/

	//(10800,720)時の座標
	DirectX::XMFLOAT2 pinPositions[10] = {
		{ 200.0f,100.0f },{400.0f,300.0f},{600.0f,100.0f},{800.0f,300.0f},{1000.0f,100.0f},
		{ 200.0f,300.0f },{400.0f,100.0f},{600.0f,300.0f},{800.0f,100.0f},{1000.0f,300.0f}
	};
	DirectX::XMFLOAT2 stagePositions[10] = {
	{150.0f, 200.0f}, {350.0f, 400.0f}, {550.0f, 200.0f}, {750.0f, 400.0f}, {950.0f, 200.0f},
	{1950.0f, 400.0f}, {2150.0f, 200.0f}, {2350.0f, 400.0f}, {2550.0f, 200.0f}, {2750.0f, 400.0f}
	};

	float scale[10] = { 1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f };
	float scrollScreenWidth = 0.0f;
	float charaMove = 0.0f;
	int selectNum = 0;
	bool moveflg=false;
};