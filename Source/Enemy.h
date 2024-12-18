#pragma once
#include"Graphics/Shader.h"
#include"Character.h"

//エネミー
class Enemy :public Character
{
public:
	Enemy(){}
	~Enemy()override{}
	//更新処理
	virtual void Update(float elapsedTime) = 0;
	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* sheder) = 0;
	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();
	virtual void DebugGui() = 0;
	//破棄
	void Destroy();
};