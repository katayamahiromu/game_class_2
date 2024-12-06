#pragma once
#include"Stage.h"
#include"Graphics/Model.h"
#include"Character.h"

class Switch : public Stage
{
public:
	Switch(DirectX::XMFLOAT3 Position,int t);
	~Switch()override;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
private:
	enum Switch_type
	{
		Hold,
		Click
	};
private:
	//今はエネミーでやってますが後で変更します
	void SwitchVsEnemy();

	//スケールで操作してるので値調整は少し面倒くさい
	float downSpeed = 0.9f;

	//スイッチのタイプ
	int type;
};