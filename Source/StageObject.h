#pragma once
#include"Stage.h"

class  AppearStage : public Stage
{
public:
	AppearStage(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 scale = {1.0f,1.0f,1.0f});
	~AppearStage()override {};
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
private:
	bool isAppear = true;
};

class Cube :  public Stage
{
public:
	Cube(DirectX::XMFLOAT3 pos = {0,0,0}, DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f });
	~Cube();

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
	void Gui();
private:
};