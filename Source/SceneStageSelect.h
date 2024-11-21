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
	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite>  yajirusi = nullptr;

	DirectX::XMFLOAT2 selectPos = { 0,600 };

	int select = 0;

	int page = 0;

	int Gui;
};