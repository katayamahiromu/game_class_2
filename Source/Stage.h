#pragma once

#include "Object.h"
#include"Graphics/Shader.h"
#include "Object.h"
#include"Collision.h"
#include"Player.h"
#include"SceneManager.h"
#include"SceneTitle.h"
#include"Character.h"


//position などTransform周りがいいのでキャラクターを継承
//ステージ
class Stage : public GameObject
{
public:
	Stage(){}
	virtual ~Stage(){}

	//更新処理
	virtual void Update(float elapsedTime) = 0;
	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//ステージとのレイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;
};