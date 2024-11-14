#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"

static Player* instace = nullptr;

//�C���X�^���X�擾
Player& Player::Instance()
{
	return *instace;
}

//�R���X�g���N�^
Player::Player() {
	//�C���X�^���X�|�C���^�擾
	instace = this;

	model = std::make_unique<Model>("Data/Model/Jammo/Jammo.mdl");
	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.0075f;

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//�ҋ@�X�e�[�g�ւ̑J��
	TransitiomIdleState();
}

//�f�X�g���N�^
Player::~Player() {
	delete hitEffect;
}

//�X�V����
void Player::Update(float elapsedTime) {
	//�X�e�[�g���̏���
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

	//�f�o�b�N�p
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

	//���͍X�V
	UpdateVelocity(elapsedTime);
	//���G���ԍX�V
	UpdateInvinciblTImer(elapsedTime);
	//�v���C���[�ƓG�Ƃ̏Փ˔���
	if (!IsPlayback) CollisionPlayerVsEnemies();
	//�I�u�W�F�N�g�s����X�V
	UpdateTranceform();
	//���f���A�j���[�V�������X�V
	model->UpdateAnimation(elapsedTime);
	//���f���s����X�V
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

//�ړ����͏���
bool Player::InputMove(float elapsedTime) {
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//�ړ�����
	Move(moveVec.x, moveVec.y, moveSpeed); // XY���ʂňړ�
	//���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.y * -1.0f, turnSpeed); // XY���ʂŐ���

	return (moveVec.x != 0.0f || moveVec.y != 0.0f);
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model.get());
}

void Player::DrawDebugGui() {
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Tranceform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�ʒu
			ImGui::InputFloat3("Position", &position.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//�X�P�[��
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

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	 // ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX() * - 1;
	float ay = gamePad.GetAxisLY();

	//// �J���������ƃX�e�B�b�N�̓��͒l�Ői�s�������v�Z
	//Camera& camera = Camera::Instance();
	//const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	//const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//// XY���ʂł̈ړ��x�N�g���v�Z
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
	vec.z = 0.0f;  // Z�����͈ړ����Ȃ�

	return vec;
}

void Player::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�Փ˗p����p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
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
	//�S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//�Փ˔���
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

//�W�����v���͏���
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

	//�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
	if (velocity.y < 1.0f) {
		//���S�A�_���[�W�X�e�[�g���͒��n�X�e�[�g�J�ڂ��Ȃ��悤�ɂ���
		if (state != State::Damage && state != State::Death) {
			TransitionLandState();
		}
	}
}

//�_���[�W���󂯂����ɌĂ΂��
void Player::OnDamaged()
{
	//�_���[�W�X�e�[�g�֑J��
	TransitionDamageState();
}

//���S�������ɌĂ΂��
void Player::OnDead()
{
	//���S�X�e�[�g�֑J��
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

//�ҋ@�X�e�[�g�֑J��
void Player::TransitiomIdleState()
{
	state = State::Idle;

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Idle, true,0.2f);
}

//�ҋ@�X�e�[�g�X�V����
void Player::UpdateIdelState(float elapsedTime)
{
	//�ړ����͏���
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	
	//�W�����v���͏���
	if (InputJump())
	{
		TransitionJumpState();
	}

	//�U���X�e�[�g
	if (InputAttack()) {
		TransitionAttackState();
	}

}

//�ړ��X�e�[�g�ւ̑J��
void Player::TransitionMoveState()
{
	state = State::Move;

	//����A�j���[�V�����̍Đ�
	model->PlayAnimation(Anime_Running, true,0.2f);
}

//�ړ��X�e�[�g�X�V����
void Player::UpdateMoveState(float elapsedTime)
{
	//�ړ����͏���
	if (!InputMove(elapsedTime))
	{
		TransitiomIdleState();
	}
	//�W�����v���͏���
	if (InputJump())
	{
		TransitionJumpState();
	}

	//�U���X�e�[�g�X�V����
	if (InputAttack()) {
		TransitionAttackState();
	}

}

//�W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
	state = State::Jump;

	//�W�����v�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Jump, false, 0.2f);
}

//�W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState(float elapsedTime)
{
	//�󒆈ړ��H
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}

	if (InputJump())
	{
		TransitionJumpState();
	}

}

//���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
	state = State::Land;

	//���n�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Landing, false, 0.2f);
}

//���n�X�e�[�g�X�V����
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

//�U���X�e�[�g�ւ̑J��
void Player::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(Anime_Attack, false, 0.2f);
}

//�U���X�e�[�g�X�V����
void Player::UpdateAttackState(float elapsedTime)
{
	if (!model->IsPlayAnimation()) {
		TransitiomIdleState();
	}
	//�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = (animationTime > 0.3 && animationTime < 0.4) ? true : false;
	if (attackCollisionFlag) {
		//����m�[�h�ƃG�l�~�[�̏Փ˔���
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
	}
}

//�m�[�h�ƓG�̏Փˏ���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	////�m�[�h�擾
	//Model::Node* node = model->FindNode(nodeName);

	////�m�[�h�ʒu�擾
	//DirectX::XMFLOAT3 nodePosition;
	//nodePosition.x = node->worldTransform._41;
	//nodePosition.y = node->worldTransform._42;
	//nodePosition.z = node->worldTransform._43;

	////�w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓ˔���
	//EnemeyManager& enemyManager = EnemeyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();
	//for (int i = 0;i < enemyCount;++i) {
	//	Enemy* enemy = enemyManager.GetEnemy(i);

	//	//�Փ˔���
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

//�_���[�W�X�e�[�g�ւ̑J��
void Player::TransitionDamageState()
{
	state = State::Damage;
	
	//�_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_GetHit1, false,0.2f);
}

//�_���[�W�X�e�[�g�X�V����
void Player::UpdateDamageState(float elapsedTime)
{
	//�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g��
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

//���S�X�e�[�g�֑J��
void Player::TransitionDeathState()
{
	state = State::Death;
	//���S�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Death, false, 0.2f);
}

//���S�X�e�[�g�X�V����
void Player::UpdateDeathState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		//�{�^�����������畜���X�e�[�g�֑J��
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			TransitionReviveState();
		}
	}
}

//�����X�e�[�g�֑J��
void Player::TransitionReviveState()
{
	state = State::Revive;
	//�̗͉�
	health = maxHealth;

	//�����A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Revive, false, 0.2f);
}

//�����X�e�[�g�X�V����
void Player::UpdateReviveState(float elapsedTime)
{
	//�����A�j���[�V�����I����ɑҋ@�X�e�[�g��
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

void Player::InitRecording()
{
	//�z��S����NULL�ŏ�����
	for (int i = 0;i < MAX_KEEP_TRANSFORM;++i)keep_position[i] = ENOUGTH;
	playback_count = 0;
}

void Player::Recording(DirectX::XMFLOAT3 position)
{
	//�z���S��������炷
	for (int i = MAX_KEEP_TRANSFORM - 1;i > 0;--i) keep_position[i] = keep_position[i - 1];
	keep_position[0] = position;
}

void Player::Playback(Character* character)
{
	//�z��̌�납��Đ�
	character->SetPositon(keep_position[playback_count]);
	playback_count++;
	if (playback_count > MAX_KEEP_TRANSFORM) playback_count = 0;
	//if (keep_position[playback_count].x == ENOUGTH.x)playback_count = 0;
}