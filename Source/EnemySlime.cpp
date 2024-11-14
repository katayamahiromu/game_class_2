#include"EnemySlime.h"
#include"Graphics/Graphics.h"
#include"Mathf.h"
#include"Player.h"

//�R���X�g���N�^
EnemySlime::EnemySlime() {
	model = std::make_unique<Model>("Data/Model/Slime/Slime.mdl");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;
	radius = 0.5f;
	height = 1.0f;

	//�p�j�X�e�[�g�֑J��
	TransitionWanderState();
}

//�f�X�g���N�^
EnemySlime::~EnemySlime() {
}

//�X�V����
void EnemySlime::Update(float elapsedTime) 
{
	//�X�e�[�g���̍X�V����
	/*switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Pursuit:
		UpdatePursuitState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	case State::IdleBattle:
		UpdateIdleBattleState(elapsedTime);
		break;
	case State::Damage:
		UpdateDamageState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	}*/
	//���f���A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	//���͍X�V����
	UpdateVelocity(elapsedTime);
	//���G���Ԃ̍X�V
	UpdateInvinciblTImer(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTranceform();
	model->UpdateTransform(transform);
}

//�`�揈��
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model.get());
}

//�_���[�W���󂯂����ɌĂ΂��
void EnemySlime::OnDamaged()
{
	//�_���[�W�X�e�[�g�֑J��
	TransitionDamageState();
}

//���S�������ɌĂ΂��
void EnemySlime::OnDead()
{
	//���S�X�e�[�g�֑J��
	TransitionDeathState();
}

//�f�o�b�N�v���~�e�B�u�`��
void EnemySlime::DrawDebugPrimitive()
{
	//���N���X�̃f�o�b�N�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 0));

	//�^�[�Q�b�g�ʒu���f�o�b�N���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

	//���G�͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

	//�U���͈͂��f�o�b�O�͈͂��~���`��
	debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//�꒣��ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

//�^�[�Q�b�g�ʒu�������_���ݒ�
void EnemySlime::SetRandomTargetPosition()
{
	float Angle = DirectX::XMConvertToRadians(Mathf::RandomRange(0, 360));
	float Length = Mathf::RandomRange(0, territoryRange);
 	targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryRange) + sinf(Angle)* Length;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryRange) + cosf(Angle)* Length;
}


//�ڕW�n�_�ֈړ�
void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	//�ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

//�v���C���[���G
bool EnemySlime::SearchPlayer()
{
	//�v���C���[�Ƃ̍��፷���l������3D�ł̋������������
	const DirectX::XMFLOAT3& playPosition = Player::Instance().GetPosition();
	float vx = playPosition.x - position.x;
	float vy = playPosition.y - position.y;
	float vz = playPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);
		//�P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;
		//�O���x�N�g��
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		//2�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f) {
			return true;
		}
	}
	return false;
}

//�m�[�h�ƃv���C���[�̏Փˏ���
void EnemySlime::CollisionNodoVsPlayer(const char* nodeName, float nodeRadius)
{
	//�m�[�h�̈ʒu�Ɠ����蔻��
	Model::Node* node = model->FindNode(nodeName);
	if (node != nullptr)
	{
		//�m�[�h�̃��[���h���W
		DirectX::XMFLOAT3 nodePosition(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
			);

		//�����蔻��\��
		Graphics::Instance().GetDebugRenderer()->DrawSphere(
			nodePosition,nodeRadius,DirectX::XMFLOAT4(0,0,1,1)
		);

		//�v���C���[�Ɠ����蔻��
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersctSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition
		))
		{
			//�_���[�W��^����
			if (player.ApplyDamage(1, 0.5f))
			{
				//�G�𐁂���΂��x�N�g�����Z�o
				DirectX::XMFLOAT3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				//XZ���ʂɐ�����΂��͂�������
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;
				//Y�����ɂ��͂�������
				vec.y = 5.0f;

				//������΂�
				player.AddImpulse(vec);
			}
		}
	}
}

//�p�j�X�e�[�g�֑J��
void EnemySlime::TransitionWanderState()
{
	state = State::Wander;

	//�ڕW�n�_�ֈړ�
	SetRandomTargetPosition();

	//�����A�j���[�V�����Đ�
	model->PlayAnimation(Anime_WalkFWD, true,0.2f);
}

//�p�j�X�e�[�g�X�V����
void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//�ڕW�n�_�܂�XZ���ʂł̋�������
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//�ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}
	//�ڕW�n�_��
	MoveToTarget(elapsedTime, 0.5f);

	if (SearchPlayer())
	{
		//����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

//�ҋ@�X�e�[�g�֑J��
void EnemySlime::TransitionIdleState()
{
	state = State::Idle;

	//�^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	//�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_IdleNormal, true,0.2f);

	if (SearchPlayer())
	{
		//����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

//�ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleState(float elaspedTime)
{
	//�^�C�}�[����
	stateTimer -= elaspedTime;
	if (stateTimer < 0.0f)
	{
		//�p�j�X�e�[�g�֑J��
		TransitionWanderState();
	}

}

//�ǐՃX�e�[�g�֑J��
void EnemySlime::TransitionPursuitState()
{
	state = State::Pursuit;

	//���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	//�����A�j���[�V����
	model->PlayAnimation(Anime_RunFWD, true,0.2f);

}

//�ǐՃX�e�[�g�X�V����
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
	//�ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = Player::Instance().GetPosition();

	//�ڕW�n�_�ֈړ�
	MoveToTarget(elapsedTime, 1.0f);

	//�^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//�ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	//�v���C���[�̋߂Â��ƍU���X�e�[�g�֑J��
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < attackRange)
	{
		//�U���X�e�[�g�֑J��
		TransitionAttackState();
	}
}

//�U���X�e�[�g�֑J��
void EnemySlime::TransitionAttackState()
{
	state = State::Attack;

	//�U���A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Attack1, false, 0.2f);
}

//�U���X�e�[�g�X�V����
void EnemySlime::UpdateAttackState(float elapsedTime)
{
	//�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	if (animationTime >= 0.1f && animationTime <= 0.35f)
	{
		//�ڋʃm�[�h�ƃv���C���[�̏Փˏ���
		CollisionNodoVsPlayer("EyeBall", 0.2f);
	}

	//�U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

void EnemySlime::TransitionIdleBattleState()
{
	state = State::IdleBattle;

	//���b�ԑҋ@����^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(2.0f, 3.0f);

	//�퓬�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_IdleBattle, true,0.2f);
}

//�퓬�ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
	//�ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = Player::Instance().GetPosition();

	//�^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		//�v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);
		if (dist < attackRange)
		{
			//�U���X�e�[�g�֑J��
			TransitionAttackState();
		}
		else
		{
			//�p�j�X�e�[�g�֑J��
			TransitionWanderState();
		}
	}

	MoveToTarget(elapsedTime,0.0f);
}

//�_���[�W�X�e�[�g�֑J��
void EnemySlime::TransitionDamageState()
{
	state = State::Damage;

	//�_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_GetHit, false, 0.2f);
}

//�_���[�W�X�e�[�g�X�V����
void EnemySlime::UpdateDamageState(float elapsedTime)
{
	//�_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}

}

//���S�X�e�[�g�֑J��
void EnemySlime::TransitionDeathState()
{
	state = State::Death;

	//�_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anime_Die, false, 0.2f);
}

//���S�X�e�[�g�X�V����
void EnemySlime::UpdateDeathState(float elapsedTime)
{
	//�_���[�W�A�j���[�V�������I������玩����j��
	if (!model->IsPlayAnimation())
	{
		Destroy();
	}
}

void EnemySlime::DebugGui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("ENEMY", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("ColorGrading", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ColorGradingData data = GetModel()->GetColorGrading();
			ImGui::SliderFloat("hueShift", &data.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &data.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brightness", &data.brightness, 0.0f, +2.0f);
			GetModel()->SetColorGrading(data);
		}
	}
	ImGui::End();
}