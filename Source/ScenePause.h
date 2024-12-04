#pragma once

#include"Scene.h"

class ScenePause : public Scene
{
public:
	ScenePause();
	~ScenePause()override {};

	// ‰Šú‰»
	void Initialize() override;

	// I—¹‰»
	void Finalize() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render() override;
};