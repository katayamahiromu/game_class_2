#pragma once

#include"Scene.h"

class ScenePause : public Scene
{
public:
	ScenePause();
	~ScenePause()override {};

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;
};