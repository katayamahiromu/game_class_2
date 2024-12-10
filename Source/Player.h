#pragma once

#include"Graphics/Shader.h"
#include"Character.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"Effect.h"

//プレイヤー
class Player :public Character
{
public:
	// タイトルで表示する用のコンストラクタ
	Player();

	Player(DirectX::XMFLOAT3 pos);
	~Player() override;

	//更新処理
	void Update(float elapsedTime);

	// タイトル用の更新処理(ほぼアニメーション再生だけ)
	void TitleUpdate(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグ用GUI描画
	void DrawDebugGui();
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	//移動入力処理
	bool InputMove(float elapsedTime);
	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	//プレイヤーとエネミーとの衝突判定
	void CollisionPlayerVsEnemies();
	//ジャンプ入力処理
	bool InputJump();
	//着地した時に呼ばれる
	void OnLanding()override;
	//攻撃入力処理
	bool InputAttack();
	//ノードとエネミーの衝突処理
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);
	//インスタンス取得
	static Player& Instance();
protected:
	//ダメージを受けた時に呼ばれる
	void OnDamaged() override;
	//死亡した時に呼ばれる
	void OnDead()override;
private:
	//待機ステートへ遷移
	void TransitiomIdleState();

	//待機ステート更新処理
	void UpdateIdelState(float elapsedTime);

	//移動ステートへ遷移
	void TransitionMoveState();

	//移動ステート更新処理
	void UpdateMoveState(float elapseTime);

	//ジャンプステートへ遷移
	void TransitionJumpState();

	//ジャンプステート更新処理
	void UpdateJumpState(float elapsedTime);

	//着地ステートへ遷移
	void TransitionLandState();

	//着地ステート更新処理
	void UpdateLandState(float elapsedTime);

	//攻撃ステートへ遷移
	void TransitionAttackState();

	//攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

	//ダメージステートへ遷移
	void TransitionDamageState();

	//ダメージステート更新処理
	void UpdateDamageState(float elapsedTime);

	//死亡ステートへ遷移
	void TransitionDeathState();

	//死亡ステート更新処理
	void UpdateDeathState(float elapsedTime);

	//復活ステートへ遷移
	void TransitionReviveState();

	//復活ステート更新処理
	void UpdateReviveState(float elapsedTime);
private:
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float JumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	Effect* hitEffect = nullptr;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	//アニメーション
	enum Animation
	{
		Anime_Attack,
		Anime_Death,
		Anime_Falling,
		Anime_GetHit1,
		Anime_GetHit2,
		Anime_Idle,
		Anime_Jump,
		Anime_Jump_Flip,
		Anime_Landing,
		Anime_Revive,
		Anime_Running,
		Anime_Walking
	};

	//ステート
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive
	};
private:
	State state = State::Idle;
public:
	//リセットの時に記録してたものも元にに戻す為にpublic
	//録画の準備
	void InitRecording();
private:
	//録画
	void Recording(DirectX::XMFLOAT3 position);
	//再生
	void Playback(Character*character);

	//static const int MAX_KEEP_TRANSFORM = 720;
	//static const int MAX_KEEP_TRANSFORM = 432;
	static const int MAX_KEEP_TRANSFORM = 1440;
	DirectX::XMFLOAT3 keep_position[MAX_KEEP_TRANSFORM];
	//録画フラグ
	bool IsRecording = false;
	//再生フラグ
	bool IsPlayback = false;
	//最新の情報は０に入っている
	int playback_count = 0;

	//何も入ってない
	DirectX::XMFLOAT3 ENOUGTH = {FLT_MAX,FLT_MAX,FLT_MAX};
};