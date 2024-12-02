#pragma once

#include"Graphics//Sprite.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>

//タイトルシーン
class SceneTitle :public Scene
{
public:
	SceneTitle(){}
	~SceneTitle(){}

	//初期化
	virtual void Initialize() override;

	//終了化
	virtual void Finalize() override;

	//更新処理
	virtual void Update(float elapsedTime) override;

	//描画処理
	virtual void Render() override;
private:
	Sprite* sprite = nullptr;
	Sprite* start = nullptr;
	Sprite* end = nullptr;
	std::unique_ptr<AudioSource> Cdur;
	std::unique_ptr<SubMixVoice> test;
	DirectX::XMFLOAT2 spritesize = { 200,100 };
	float scale[2] = {1.1f,1.0f};
	bool finalizeflg = false;
};