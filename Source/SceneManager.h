#pragma once
#include"Scene.h"

//�V�[���}�l�[�W���[
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	//�B��̃C���X�^���X�擾
	static SceneManager& instance()
	{
		static SceneManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);
	//�`�揈��
	void Render();
	//�V�[���N���A
	void Clear();
	//�V�[���؂�ւ�
	void ChengeScene(Scene* scene);

	int select = 0;
private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
};
