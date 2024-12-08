#pragma once
#include"Stage.h"
#include"Graphics/Graphics.h"

class Goal : public GameObject
{
public:
	Goal(DirectX::XMFLOAT3 position);
	~Goal();
	void Update(float elapsedTime) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);
private:
	void PlayerVsGoal();

	//“–‚½‚è”»’è—p‚Ì”¼Œa
	float radius = 0.5f;
};