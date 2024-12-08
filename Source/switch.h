#pragma once
#include "Object.h"
#include"Graphics/Model.h"
#include"Character.h"
#include "Graphics/Shader.h"
#include "Collision.h"

class ClickSwitch : public GameObject
{
public:
	ClickSwitch(DirectX::XMFLOAT3 Position);
	~ClickSwitch()override;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
private:
	//スイッチが押された時の動き
	void OnSwitchMove();
	//今はエネミーでやってますが後で変更します
	void SwitchVsEnemy();

	//スケールで操作してるので値調整は少し面倒くさい
	float downSpeed = 0.9f;

	//押されたフラグ
	bool pushFlag = false;
};


class HoldSwitch : public GameObject
{
public:
	HoldSwitch(DirectX::XMFLOAT3 Position);
	~HoldSwitch()override;

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
private:
	//スイッチが押された時の動き
	void OnSwitchMove();
	//今はエネミーでやってますが後で変更します
	void SwitchVsEnemy();

	//スケールで操作してるので値調整は少し面倒くさい
	float downSpeed = 0.9f;

	//押されたフラグ
	bool pushFlag = false;
};