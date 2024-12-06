#pragma once
#include"Stage.h"

class  ObjectStage : public Stage
{
public:
	ObjectStage(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale = {1.0f,1.0f,1.0f});
	~ObjectStage()override {};
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
private:
	bool isAppear = true;
};
