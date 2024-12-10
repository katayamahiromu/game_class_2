#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>

//���[�h�f�B���O�V�[��
class SceneLoading :public Scene
{
public:
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;
private:
	//���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);
private:
	std::unique_ptr<Sprite> kaiten = nullptr;
	std::unique_ptr<Sprite> loading = nullptr;
	std::unique_ptr<Sprite> back = nullptr;

	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
};