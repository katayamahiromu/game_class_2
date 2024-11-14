#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"

static Player* instace = nullptr;

//インスタンス取得
Player& Player::Instance()
{
	return *instace;
}

//コンストラクタ
Player::Player() {
	//インスタンスポインタ取得
	instace = this;

	model = std::make_unique<Model>("Data/Model/Jammo/Jammo.mdl");
	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.0075f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//待機ステートへの遷移
	TransitiomIdleState();
}

//デストラクタ
Player::~Player() {
	delete hitEffect;
}

//更新処理
void Player::Update(float elapsedTime) {
	//ステート毎の処理
	switch (state)
	{
	case State::Idle:
		UpdateIdelState(elapsedTime);
		break;
	case State::Move:
		UpdateMoveState(elapsedTime);
		break;
	case State::Jump:
		UpdateJumpState(elapsedTime);
		break;
	case State::Land:
		UpdateLandState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	case State::Damage:
		UpdateDamageState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	case State::Revive:
		UpdateReviveState(elapsedTime);
		break;
	}

	//デバック用
	{
		if (GetAsyncKeyState('M') & 0x8000)
		{
			InitRecording();
			IsRecording = true;
		}

		if (IsRecording) Recording(EnemeyManager::Instance().GetEnemy(0)->GetPosition());
		if (GetAsyncKeyState('N') & 0x8000)
		{
			IsPlayback = true;
			IsRecording = false;
		}
		if (IsPlayback)Playback(EnemeyManager::Instance().GetEnemy(0));
	}

	//速力更新
	UpdateVelocity(elapsedTime);
	//無敵時間更新
	UpdateInvinciblTImer(elapsedTime);
	//プレイヤーと敵との衝突判定
	if (!IsPlayback) CollisionPlayerVsEnemies();
	//オブジェクト行列を更新
	UpdateTranceform();
	//モデルアニメーションを更新
	model->UpdateAnimation(elapsedTime);
	//モデル行列を更新
	model->UpdateTransform(transform);

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		ToggleMoveMode();

		hitEffect->Play(position);
	}

	if (isXYMode)
	{
		angle.x = DirectX::XMConvertToRadians(90);
	}
	else
	{
		angle.x = 0;
	}

	position.z = 1.502f;
}

//移動入力処理
bool Player::InputMove(float elapsedTime) {
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//移動処理
	Move(moveVec.x, moveVec.y, moveSpeed); // XY平面で移動
	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.y * -1.0f, turnSpeed); // XY平面で旋回

	return (moveVec.x != 0.0f || moveVec.y != 0.0f);
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model.get());
}

void Player::DrawDebugGui() {
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
		//トランスフォーム
		if (ImGui::CollapsingHeader("Tranceform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);

			ColorGradingData data = EnemeyManager::Instance().GetEnemy(0)->GetModel()->GetColorGrading();
			ImGui::SliderFloat("hueShift", &data.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &data.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brightness", &data.brightness, 0.0f, +2.0f);
			EnemeyManager::Instance().GetEnemy(0)->GetModel()->SetColorGrading(data);
		}
	}
	ImGui::End();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	 // 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX() * - 1;
	float ay = gamePad.GetAxisLY();

	//// カメラ方向とスティックの入力値で進行方向を計算
	//Camera& camera = Camera::Instance();
	//const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	//const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//// XY平面での移動ベクトル計算
	//float cameraRightX = cameraRight.x;
	//float cameraRightY = cameraRight.y;
	//float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightY * cameraRightY);
	//if (cameraRightLength > 0.0f) {
	//	cameraRightX /= cameraRightLength;
	//	cameraRightY /= cameraRightLength;
	//}

	//float cameraFrontX = cameraFront.x;
	//float cameraFrontY = cameraFront.y;
	//float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontY * cameraFrontY);
	//if (cameraFrontLength > 0.0f) {
	//	cameraFrontX /= cameraFrontLength;
	//	cameraFrontY /= cameraFrontLength;
	//}

	DirectX::XMFLOAT3 vec;
	vec.x = /*cameraFrontX * ay + cameraRightX * */ax;
	vec.y = /*cameraFrontY * ay + cameraRightY * */ay;
	vec.z = 0.0f;  // Z方向は移動しない

	return vec;
}

void Player::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//衝突用判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//攻撃衝突用の左手ノードのデバッグ球を描画
	if (attackCollisionFlag) {
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(DirectX::XMFLOAT3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}
}

void Player::CollisionPlayerVsEnemies() {
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//衝突判定
		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		)) {
			enemy->SetPositon(outPosition);
		}
	};
}

//ジャンプ入力処理
bool Player::InputJump() {
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (!isXYMode)
	{
		if (gamePad.GetButtonDown() & GamePad::BTN_A) {
			jumpCount++;
			if (jumpCount < jumpLimit) Junp(JumpSpeed);
			if (isGround) {
				jumpCount = 0;
				return true;
			}
		}
	}
	return false;
}

void Player::OnLanding() {
	isGround = true;

	//下方向の速力が一定以上なら着地ステートへ
	if (velocity.y < 1.0f) {
		//死亡、ダメージステート時は着地ステート遷移しないようにする
		if (state != State::Damage && state != State::Death) {
			TransitionLandState();
		}
	}
}

//ダメージを受けた時に呼ばれる
void Player::OnDamaged()
{
	//ダメージステートへ遷移
	TransitionDamageState();
}

//死亡した時に呼ばれる
void Player::OnDead()
{
	//死亡ステートへ遷移
	TransitionDeathState();
}


bool Player::InputAttack()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}
	return false;
}

//待機ステートへ遷移
void Player::TransitiomIdleState()
{
	state = State::Idle;

	//待機アニメーション再生
	model->PlayAnimation(Anime_Idle, true,0.2f);
}

//待機ステート更新処理
void Player::UpdateIdelState(float elapsedTime)
{
	//移動入力処理
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}

	//攻撃ステート
	if (InputAttack()) {
		TransitionAttackState();
	}

}

//移動ステートへの遷移
void Player::TransitionMoveState()
{
	state = State::Move;

	//走りアニメーションの再生
	model->PlayAnimation(Anime_Running, true,0.2f);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
	//移動入力処理
	if (!InputMove(elapsedTime))
	{
		TransitiomIdleState();
	}
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}

	//攻撃ステート更新処理
	if (InputAttack()) {
		TransitionAttackState();
	}

}

//ジャンプステートへ遷移
void Player::TransitionJumpState()
{
	state = State::Jump;

	//ジャンプアニメーション再生
	model->PlayAnimation(Anime_Jump, false, 0.2f);
}

//ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
	//空中移動？
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}

	if (InputJump())
	{
		TransitionJumpState();
	}

}

//着地ステートへ遷移
void Player::TransitionLandState()
{
	state = State::Land;

	//着地アニメーション再生
	model->PlayAnimation(Anime_Landing, false, 0.2f);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	else 
	{
		TransitiomIdleState();
	}


	if (InputJump())
	{
		TransitionJumpState();
	}
}

//攻撃ステートへの遷移
void Player::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(Anime_Attack, false, 0.2f);
}

//攻撃ステート更新処理
void Player::UpdateAttackState(float elapsedTime)
{
	if (!model->IsPlayAnimation()) {
		TransitiomIdleState();
	}
	//任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = (animationTime > 0.3 && animationTime < 0.4) ? true : false;
	if (attackCollisionFlag) {
		//左手ノードとエネミーの衝突判定
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
	}
}

//ノードと敵の衝突処理
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	////ノード取得
	//Model::Node* node = model->FindNode(nodeName);

	////ノード位置取得
	//DirectX::XMFLOAT3 nodePosition;
	//nodePosition.x = node->worldTransform._41;
	//nodePosition.y = node->worldTransform._42;
	//nodePosition.z = node->worldTransform._43;

	////指定のノードと全ての敵を総当たりで衝突判定
	//EnemeyManager& enemyManager = EnemeyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();
	//for (int i = 0;i < enemyCount;++i) {
	//	Enemy* enemy = enemyManager.GetEnemy(i);

	//	//衝突判定
	//	DirectX::XMFLOAT3 outPosition;
	//	if (Collision::IntersctSphereVsCylinder(
	//		nodePosition,
	//		nodeRadius,
	//		enemy->GetPosition(),
	//		enemy->GetRadius(),
	//		enemy->GetHeight(),
	//		outPosition)
	//		)
	//	{
	//		enemy->SetPositon(outPosition);
	//	}
	//}
}

//ダメージステートへの遷移
void Player::TransitionDamageState()
{
	state = State::Damage;
	
	//ダメージアニメーション再生
	model->PlayAnimation(Anime_GetHit1, false,0.2f);
}

//ダメージステート更新処理
void Player::UpdateDamageState(float elapsedTime)
{
	//ダメージアニメーションが終わったら待機ステートへ
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

//死亡ステートへ遷移
void Player::TransitionDeathState()
{
	state = State::Death;
	//死亡アニメーション再生
	model->PlayAnimation(Anime_Death, false, 0.2f);
}

//死亡ステート更新処理
void Player::UpdateDeathState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		//ボタンを押したら復活ステートへ遷移
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			TransitionReviveState();
		}
	}
}

//復活ステートへ遷移
void Player::TransitionReviveState()
{
	state = State::Revive;
	//体力回復
	health = maxHealth;

	//復活アニメーション再生
	model->PlayAnimation(Anime_Revive, false, 0.2f);
}

//復活ステート更新処理
void Player::UpdateReviveState(float elapsedTime)
{
	//復活アニメーション終了後に待機ステートへ
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

void Player::InitRecording()
{
	//配列全部をNULLで初期化
	for (int i = 0;i < MAX_KEEP_TRANSFORM;++i)keep_position[i] = ENOUGTH;
	playback_count = 0;
}

void Player::Recording(DirectX::XMFLOAT3 position)
{
	//配列を全部一つずつずらす
	for (int i = MAX_KEEP_TRANSFORM - 1;i > 0;--i) keep_position[i] = keep_position[i - 1];
	keep_position[0] = position;
}

void Player::Playback(Character* character)
{
	//配列の後ろから再生
	character->SetPositon(keep_position[playback_count]);
	playback_count++;
	if (playback_count > MAX_KEEP_TRANSFORM) playback_count = 0;
	//if (keep_position[playback_count].x == ENOUGTH.x)playback_count = 0;
}