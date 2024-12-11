#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"
#include "StageManager.h"

static Player* instace = nullptr;

//インスタンス取得
Player& Player::Instance()
{
	return *instace;
}

// タイトルで表示するようのコンストラクタ
Player::Player()
{
	//インスタンスポインタ取得
	instace = this;
	position = {0.0f,-2.0f,0.0f};
	model = std::make_unique<Model>("Data/Model/Robbot/robot.mdl");
	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.02f;
}

//コンストラクタ
Player::Player(DirectX::XMFLOAT3 pos) {
	//インスタンスポインタ取得
	instace = this;
	position = pos;
	model = std::make_unique<Model>("Data/Model/Robbot/robot.mdl");
	angle.y = 90.0f;
	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.02f;

	height = 0.5f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//待機ステートへの遷移
	TransitiomIdleState();

	//処理順に左右されるので気をつけてください
	//プレイヤーの生成は必ず最後にしてください
	Model* model = EnemeyManager::Instance().GetEnemy(0)->GetModel();
	ColorGradingData data = model->GetColorGrading();
	data.hueShift = 328.0f;
	model->SetColorGrading(data);

	recordStart = Audio::Instance().LoadAudioSource("Data/Audio/SE/record.wav");
	foot = Audio::Instance().LoadAudioSource("Data/Audio/SE/foot.wav");
	foot->Set_Volume(0.45f);
	objectMove = Audio::Instance().LoadAudioSource("Data/Audio/SE/object_move.wav");
	objectMove->Set_Volume(0.45f);
	XYCheneg = Audio::Instance().LoadAudioSource("Data/Audio/SE/choice.wav");
	XYCheneg->Set_Volume(0.1f);

	//今回はアニメーションが一つしかないため直書き
	model->PlayAnimation(0, true, 0.2f);
}

//デストラクタ
Player::~Player() 
{
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
	/*case State::Move:
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
		break;*/
	}

	//デバック用
	{
		if (GetAsyncKeyState('M') & 0x8000)
		{
			InitRecording();
			isRecording = true;
			isOk = true;
			recordStart->DC_Play();
		}

		if (isRecording) Recording(EnemeyManager::Instance().GetEnemy(0)->GetPosition());

		if (GetAsyncKeyState('N') & 0x8000)
		{
			if (!isOk)return;
			isPlayback = true;
			isRecording = false;
			playback_count = 0;
		}


		ColorGradingData data = EnemeyManager::Instance().GetEnemy(0)->GetModel()->GetColorGrading();
		if (isPlayback)
		{
			Playback(EnemeyManager::Instance().GetEnemy(0));
			data.saturation = 0.0f;
		}
		else
		{
			data.saturation = 1.0f;
		}
		EnemeyManager::Instance().GetEnemy(0)->GetModel()->SetColorGrading(data);
	}

	//速力更新
	UpdateVelocity(elapsedTime);
	//無敵時間更新
	UpdateInvinciblTImer(elapsedTime);
	//プレイヤーと敵との衝突判定
	CollisionPlayerVsEnemies(elapsedTime);
	//オブジェクト行列を更新
	UpdateTransform();
	//モデルアニメーションを更新
	model->UpdateAnimation(elapsedTime);
	//モデル行列を更新
	model->UpdateTransform(transform);

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (InputAttack())
	{
		ToggleMoveMode();
		XYCheneg->DC_Play();

		// モード切替時に壁に埋まらないようにY座標を補正する
		if (isXYMode)
		{
			DirectX::XMFLOAT3 start{ position.x, position.y + 0.2f, position.z }, end{ start.x, position.y - radius, start.z };
			HitResult dummy;
			if (StageManager::Instance().RaycastToStage(start, end, dummy))
			{
				position.y += radius;
			}
		}

		//hitEffect->Play(position);
	}

	if (isXYMode)
	{
		angle.x = DirectX::XMConvertToRadians(-90);
	}
	else
	{
		angle.x = 0;
	}

	BoxSpark(elapsedTime);
}

// タイトル用のUpdate
void Player::TitleUpdate(float elapsedTime)
{
	//オブジェクト行列を更新
	UpdateTransform();
	//モデルアニメーションを更新
	model->UpdateAnimation(elapsedTime);
	//モデル行列を更新
	model->UpdateTransform(transform);
}

//移動入力処理
bool Player::InputMove(float elapsedTime) {
	//進行ベクトル取得
	moveVec = GetMoveVec();
	moveVec.y = isXYMode ? moveVec.y : 0; //当然2Dモードにy入力はいらんよなぁ

	//足音
	(moveVec.x != 0.0f || moveVec.y != 0.0f)? foot->Play(false): foot->Stop();
	

	//移動処理
	Move(moveVec.x, moveVec.y, moveSpeed); // XY平面で移動
	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.y * -1.0f, turnSpeed); // XY平面で旋回

	return (moveVec.x != 0.0f || moveVec.y != 0.0f);
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	position.y += 0.5f;
	UpdateTransform();
	model->UpdateTransform(transform);

	shader->Draw(dc, model.get());

	position.y -= 0.5f;
	UpdateTransform();
	model->UpdateTransform(transform);
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
			ImGui::InputFloat3("velocity", &velocity.x);

			ColorGradingData data = EnemeyManager::Instance().GetEnemy(0)->GetModel()->GetColorGrading();
			ImGui::SliderFloat("hueShift", &data.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &data.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brightness", &data.brightness, 0.0f, +2.0f);
			EnemeyManager::Instance().GetEnemy(0)->GetModel()->SetColorGrading(data);

			StageManager& manager = StageManager::Instance();
			int i = manager.GetPushCount();
			ImGui::InputInt("push Count", &i);
		}
	}
	ImGui::End();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	 // 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX() ;
	float ay = gamePad.GetAxisLY() ;

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
	//debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//攻撃衝突用の左手ノードのデバッグ球を描画
	/*if (attackCollisionFlag) {
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(DirectX::XMFLOAT3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}*/
}


void Player::CollisionPlayerVsEnemies(float elapsedTime) {
	EnemeyManager& enemyManager = EnemeyManager::Instance();

	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) 
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//再生中にエネミーはプレイヤーを押し出し、そうでなければプレイヤーが敵を押し出す
		if (this->isPlayback)
		{
			/*if (Collision::IntersectCubeVsCube(enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
				this->position, this->radius, this->height, outPosition
			))
			{
				const float offset = 0.1f;
				this->position.y = outPosition.y + offset;
			}*/

			//プレイヤーとエネミーでレイキャスト
			DirectX::XMFLOAT3 start = { this->position.x, this->position.y + stepOffset, this->position.z };
			DirectX::XMFLOAT3 end = { this->position.x, this->position.y + this->velocity.y * elapsedTime, this->position.z };
			HitResult hitResult{};
			if (Collision::InterserctRayVsModel(start, end, enemy->GetModel(), hitResult))
			{
				velocity.y = 0;
				position = hitResult.position;
				//ヒットした位置をエネミーのローカル空間に変換する
				DirectX::XMMATRIX FloorIM = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&enemy->GetTransform()));
				DirectX::XMVECTOR CharLP = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&this->position), FloorIM);

				//エネミーのワールド行列を更新する
				enemy->UpdateTransform();

				//移動後のエネミーの行列を使用して上で求めたローカル位置をワールド位置に変換する
				DirectX::XMVECTOR CharWP = DirectX::XMVector3Transform(CharLP, DirectX::XMLoadFloat4x4(&enemy->GetTransform()));
				DirectX::XMStoreFloat3(&this->position, CharWP);

				//進行ベクトルがXY共に0.0fの時にX軸方向についていくようにする
				if (moveVec.x == 0.0f && moveVec.y == 0.0f)
				{
					position.x = enemy->GetPosition().x;
				}

			}

		}
		else
		{
			//当たったら音も鳴らす
			if (Collision::IntersectCubeVsCube(this->position, this->radius, this->height,
				enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(), outPosition
			))
			{
				enemy->SetPosition(outPosition);
				objectMove->Play(false);
			}
			else
			{
				objectMove->Stop();
			}

		}
	}
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

	////下方向の速力が一定以上なら着地ステートへ
	//if (velocity.y < 1.0f) {
	//	//死亡、ダメージステート時は着地ステート遷移しないようにする
	//	if (state != State::Damage && state != State::Death) {
	//		TransitionLandState();
	//	}
	//}
}

//ダメージを受けた時に呼ばれる
void Player::OnDamaged()
{
	////ダメージステートへ遷移
	//TransitionDamageState();
}

//死亡した時に呼ばれる
void Player::OnDead()
{
	////死亡ステートへ遷移
	//TransitionDeathState();
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
	model->PlayAnimation(0, true,0.2f);
}

//待機ステート更新処理
void Player::UpdateIdelState(float elapsedTime)
{
	//移動入力処理
	if (InputMove(elapsedTime))
	{
		//TransitionMoveState();
	}
	
	//ジャンプ入力処理
	if (InputJump())
	{
		//TransitionJumpState();
	}

	//攻撃ステート
	if (InputAttack()) {
		//TransitionAttackState();
	}

}

////移動ステートへの遷移
//void Player::TransitionMoveState()
//{
//	state = State::Move;
//
//	//走りアニメーションの再生
//	//model->PlayAnimation(Anime_Running, true,0.2f);
//}

////移動ステート更新処理
//void Player::UpdateMoveState(float elapsedTime)
//{
//	////移動入力処理
//	//if (!InputMove(elapsedTime))
//	//{
//	//	TransitiomIdleState();
//	//}
//	////ジャンプ入力処理
//	//if (InputJump())
//	//{
//	//	TransitionJumpState();
//	//}
//
//	////攻撃ステート更新処理
//	//if (InputAttack()) {
//	//	TransitionAttackState();
//	//}
//
//}

////ジャンプステートへ遷移
//void Player::TransitionJumpState()
//{
//	state = State::Jump;
//
//	//ジャンプアニメーション再生
//	//model->PlayAnimation(Anime_Jump, false, 0.2f);
//}

////ジャンプステート更新処理
//void Player::UpdateJumpState(float elapsedTime)
//{
//	////空中移動？
//	//if (InputMove(elapsedTime))
//	//{
//	//	TransitionMoveState();
//	//}
//
//	//if (InputJump())
//	//{
//	//	TransitionJumpState();
//	//}
//
//}

////着地ステートへ遷移
//void Player::TransitionLandState()
//{
//	state = State::Land;
//
//	//着地アニメーション再生
//	//model->PlayAnimation(Anime_Landing, false, 0.2f);
//}
//
////着地ステート更新処理
//void Player::UpdateLandState(float elapsedTime)
//{
//	/*if (InputMove(elapsedTime))
//	{
//		TransitionMoveState();
//	}
//	else 
//	{
//		TransitiomIdleState();
//	}
//
//
//	if (InputJump())
//	{
//		TransitionJumpState();
//	}*/
//}

////攻撃ステートへの遷移
//void Player::TransitionAttackState()
//{
//	state = State::Attack;
//	model->PlayAnimation(Anime_Attack, false, 0.2f);
//}

////攻撃ステート更新処理
//void Player::UpdateAttackState(float elapsedTime)
//{
//	if (!model->IsPlayAnimation()) {
//		TransitiomIdleState();
//	}
//	//任意のアニメーション再生区間でのみ衝突判定処理をする
//	float animationTime = model->GetCurrentAnimationSeconds();
//	attackCollisionFlag = (animationTime > 0.3 && animationTime < 0.4) ? true : false;
//	if (attackCollisionFlag) {
//		//左手ノードとエネミーの衝突判定
//		//CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
//	}
//}

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

////ダメージステートへの遷移
//void Player::TransitionDamageState()
//{
//	state = State::Damage;
//	
//	//ダメージアニメーション再生
//	//model->PlayAnimation(Anime_GetHit1, false,0.2f);
//}
//
////ダメージステート更新処理
//void Player::UpdateDamageState(float elapsedTime)
//{
//	//ダメージアニメーションが終わったら待機ステートへ
//	if (!model->IsPlayAnimation())
//	{
//		TransitiomIdleState();
//	}
//}
//
//
////死亡ステートへ遷移
//void Player::TransitionDeathState()
//{
//	state = State::Death;
//	//死亡アニメーション再生
//	//model->PlayAnimation(Anime_Death, false, 0.2f);
//}
//
////死亡ステート更新処理
//void Player::UpdateDeathState(float elapsedTime)
//{
//	if (!model->IsPlayAnimation())
//	{
//		//ボタンを押したら復活ステートへ遷移
//		GamePad& gamePad = Input::Instance().GetGamePad();
//		if (gamePad.GetButtonDown() & GamePad::BTN_A)
//		{
//			TransitionReviveState();
//		}
//	}
//}
//
////復活ステートへ遷移
//void Player::TransitionReviveState()
//{
//	state = State::Revive;
//	//体力回復
//	health = maxHealth;
//
//	//復活アニメーション再生
//	//model->PlayAnimation(Anime_Revive, false, 0.2f);
//}
//
////復活ステート更新処理
//void Player::UpdateReviveState(float elapsedTime)
//{
//	//復活アニメーション終了後に待機ステートへ
//	if (!model->IsPlayAnimation())
//	{
//		TransitiomIdleState();
//	}
//}


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
	character->SetPosition(keep_position[playback_count]);
	playback_count++;
	if (playback_count > MAX_KEEP_TRANSFORM) isPlayback = false;
	if (keep_position[playback_count].x == ENOUGTH.x)isPlayback = false;
}

void Player::BoxSpark(float elapsedTime)
{
	Model* model = EnemeyManager::Instance().GetEnemy(0)->GetModel();
	ColorGradingData data = model->GetColorGrading();

	if (isParamUp)
	{
		data.brightness += elapsedTime / 5.0f;
		if (data.brightness > UP_LIMITE) isParamUp = false;
	}
	else
	{
		data.brightness -= elapsedTime / 5.0f;
		if (data.brightness < DOWN_LIMITE) isParamUp = true;
	}
	model->SetColorGrading(data);
}