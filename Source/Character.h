#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"
#include<memory>

//キャラクター
class Character
{
public:
	Character() {}
	virtual ~Character() {}
	void UpdateTranceform();

	//位置情報取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	//スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	//半径取得
	float GetRadius() const { return radius; }
	//移動処理
	void Move(float vx, float vz, float speed);
	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	//ジャンプ処理
	void Junp(float speed);
	//速力処理更新
	void UpdateVelocity(float elapsedTime);
	//地面に接地しているか
	bool IsGround() const { return isGround; }
	//高さ取得
	float GetHeight() const { return height; }
	//ダメージを与える
	bool ApplyDamage(int damage,float invincibleTime);
	//衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);
	//健康状態を取得
	int GetHealth() const { return health; }
	//最大健康状態を取得
	int GetMaxhealth() const { return maxHealth; }

	Model* GetModel() { return model.get(); }


	void ToggleMoveMode() { isXYMode = !isXYMode; } // 移動モードの切り替え


	void SetTransform(DirectX::XMFLOAT4X4 trans) { transform = trans; }
	DirectX::XMFLOAT4X4& GetTransform() {return transform; }

protected:
	bool isXYMode = false; // XY移動モードフラグ
	DirectX::XMFLOAT3   position = { 0,0,0 };
	DirectX::XMFLOAT3   angle = { 0,0,0 };
	DirectX::XMFLOAT3   scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	float radius  =  0.5;
	float gravity = -1.0f;
	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	//着地したときに呼ばれる
	virtual void OnLanding() {};
	float height  =  2.0f;

	//ダメージを受けた時に呼ばれる
	virtual void OnDamaged(){}
	//死亡した時に呼ばれる
	virtual void OnDead(){}
	//無敵時間更新
	void UpdateInvinciblTImer(float elapsedTime);
private:
	//垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);
	//垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);
	//水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);
	//水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);

protected:
	bool isGround = false;
	int health = 5;
	int maxHealth = 5;
	float invicibleTime = 1.0f;
	float friction = 0.5f;
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecY = 0.0f;
	float airControl = 0.3f;
	float stepOffset = 1.0f;
	float slopeRate = 1.0f;

	std::unique_ptr<Model>model;
};