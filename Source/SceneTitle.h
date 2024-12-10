#pragma once

#include"Graphics//Sprite.h"
#include"Graphics/Model.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>

#include"Player.h"

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

	//Sprite* sprite = nullptr;
	std::unique_ptr<Sprite> rogo = nullptr;
	std::unique_ptr<Sprite> press_enter = nullptr;
	std::unique_ptr<Sprite> press_bar = nullptr;

	std::unique_ptr<Sprite>back;

	std::unique_ptr<Player>player;

	std::unique_ptr<AudioSource>BGM;
	std::unique_ptr<AudioSource>decide;
	bool chengFlag = false;
};