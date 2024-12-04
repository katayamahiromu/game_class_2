#pragma once

#include"Scene.h"

class ScenePause : public Scene
{
public:
	ScenePause();
	~ScenePause()override {};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
};