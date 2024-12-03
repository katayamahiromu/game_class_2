#pragma once
#include"Stage.h"
#include"Graphics/Graphics.h"

class Goal :public Stage
{
public:
	Goal(DirectX::XMFLOAT3 position);
	~Goal();
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
};