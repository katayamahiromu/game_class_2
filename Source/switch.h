#pragma once
#include "Object.h"
#include"Graphics/Model.h"
#include"Character.h"
#include "Graphics/Shader.h"
#include "Collision.h"

class Switch : public GameObject
{
public:
	Switch(DirectX::XMFLOAT3 Position);
	~Switch();

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);
private:
	//今はエネミーでやってますが後で変更します
	void SwitchVsEnemy();
	//スケールで操作してるので値調整は少し面倒くさい
	float downSpeed = 0.9f;

	//当たり判定用の半径
	float radius = 0.5f;
};