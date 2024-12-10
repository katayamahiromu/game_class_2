#pragma once

#include"Graphics//Sprite.h"
#include"Graphics/Model.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>

#include"Player.h"

//�^�C�g���V�[��
class SceneTitle :public Scene
{
public:
	SceneTitle(){}
	~SceneTitle(){}

	//������
	virtual void Initialize() override;

	//�I����
	virtual void Finalize() override;

	//�X�V����
	virtual void Update(float elapsedTime) override;

	//�`�揈��
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