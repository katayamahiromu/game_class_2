#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"

//�X�V�s��
void Character::UpdateTranceform() {
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s����쐬

	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//�R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	//�v�Z�������[���h�����o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

//�ړ�����
void Character::Move(float vx, float vz, float speed) {
	//�������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;
	//�ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;
	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v����
	if (vx == 0.0f && vz == 0.0f)return;
	float lenth = sqrtf(vx * vx + vz * vz);

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= lenth;
	vz /= lenth;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (vx * frontX) + (vz * frontZ); //����

	//���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	//�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot; //�␳�l
	if (rot > speed) rot = speed;

	//���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (vz * frontX) - (vx * frontZ);

	//2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E������s����
	//���E������s�����ɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

//�W�����v����
void Character::Junp(float speed) {
	//������̗͂�ݒ�
	velocity.y = speed;
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//���͂�͂ɉ�����
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

//���͏����X�V
void Character::UpdateVelocity(float elapsedTime) {
	//�o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	//�������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);
	//�������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);
	//�����ړ��X�V����
	UpdateVerticalMove(elapsedTime);
	//�����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);
}

bool Character::ApplyDamage(int damage,float invincibleTime)
{
	//�_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0) return false;
	//���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health < 0) return false;
	//�_���[�W����
	if (invincibleTime > 0) {
		health -= damage;
		//���S�ʒm
		if (health <= 0) {
			OnDead();
		}
		//�_���[�W�ʒm
		else {
			OnDamaged();
			invicibleTime = invincibleTime;
		}
	}
	//���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}

void Character::UpdateInvinciblTImer(float elapsedTime)
{
	if (invicibleTime > 0.0f) {
		invicibleTime -= elapsedTime;
	}
}

//�������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	if (!isXYMode)
	{
		//�d�͏���
		velocity.y += gravity * elapsedFrame;;
	}
}

//�����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
	//���������̈ړ���
	float my = velocity.y * elapsedTime;

	slopeRate = 0.0f;
	//�L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0,1,0 };

	//������
	if (my < 0.0f)
	{
		//���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		//���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

		//���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//�@���x�N�g�����擾
			normal = hit.normal;

			//�n�ʂɐڒn���Ă���
			position = hit.position;
			//��]
			angle.y += hit.rotation.y;
			//�X�΂̌v�Z
			slopeRate = 1.0f - (hit.normal.y /
				(hit.normal.y + sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z)));

			//���n����
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;


		}
		else
		{
			//�󒆂ɕ����Ă���
			position.y += my;
			isGround = false;
		}
	}
	//�㏸��
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}

	//�n�ʂ̌����ɉ����悤��XZ����]
	{
		//Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����

		float angleX = atan2f(normal.z, normal.y);
		float angleZ = -atan2f(normal.x, normal.y);

		//���`�⊮�Ŋ��炩�ɉ�]����
		if (angleX <= 1.0f && angleX >= -1.0f) {
			angle.x = Mathf::Leap(angle.x, angleX, elapsedTime * 10.0f);
		}
		if (angleZ <= 1.0f && angleZ >= -1.0f) {
			angle.z = Mathf::Leap(angle.z, angleZ, elapsedTime * 10.0f);
		}
	}
}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ���ʂ̑��͂���������
	//�O�����Œ�������낤
	float length = sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y));
	if (length > 0.0f) 
	{
		//���C��
		float friction = this->friction * elapsedFrame;
		//�󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!IsGround()) friction -= this->airControl;

		//���C�ɂ�鉡�����̌�������
		if(length > friction)
		{
			//�P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.y / length;
			//�P�ʃx�N�g�����������͂𖀎C�͕��X�P�[�����O�����l�𑬗͂������
			velocity.x -= vx * friction;
			velocity.y -= vz * friction;
		}
		//�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0;
			velocity.y = 0;
		}
	}

	//XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		//�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf((moveVecX * moveVecX) + (moveVecZ * moveVecZ));
		if(moveVecLength > 0.0f)
		{
			//������
			float acceleration = this->acceleration * elapsedFrame;
			//�󒆂ɂ���Ƃ��͉����͂����炷
			if (!IsGround()) acceleration -= this->airControl;
			//�ړ��x�N�g���ɂ���������
			velocity.x += moveVecX * acceleration;
			velocity.y += moveVecZ * acceleration;

			//�ő呬�x����
			float length = sqrtf((velocity.x * velocity.x) + (velocity.y * velocity.y));
			if (length > maxMoveSpeed)
			{
				//�ő呬�x�𒴂����瑬�x��P�ʃx�N�g���i�P�j�ɍő呬�x�������Đ�����
				velocity.x *= maxMoveSpeed / length;
				velocity.y *= maxMoveSpeed / length;
			}
		}

		//����K�^�K�^���Ȃ��悤�ɂ���
		if (IsGround() && slopeRate > 0.0f)
		{
			velocity.y -= length * slopeRate;
		}
	}
	//�ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//�������͌v�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
	if (velocityLengthXZ > 0.0f)
	{
		//�����ړ�
		float mx = velocity.x * elapsedTime;
		float mz = velocity.y * elapsedTime;

		//���C�̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };

		//���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//�ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
			//�ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

			//�␳�ʒu�̌v�Z
			float distance = 0;
			DirectX::XMStoreFloat(&distance, Dot);

			//�@���̑傫���������ɍ��킹��
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(Vec, DirectX::XMVectorScale(Normal, distance));

			DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
			Position = DirectX::XMVectorAdd(Position, R);
			DirectX::XMStoreFloat3(&position, Position);
		}
		else 
		{
			//�ړ�
			position.x += mx;
			if (isXYMode)
			{
				position.y += mz;
			}
		}

	}
}