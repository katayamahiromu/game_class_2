#include"StageMoveFloor.h"

//�R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5;

	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
	//�X�e�[�W���f����j��
	delete model;
}

//�X�V����
void StageMoveFloor::Update(float elapsedTime)
{
	//�O��̏���ۑ�
	OldTransform = transform;

	//�X�^�[�g����S�[���܂ł̋������Z�o����
	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
	float length;
	DirectX::XMStoreFloat(&length, Length);
	//�X�^�[�g����S�[���܂ł̊Ԃ���b�ԂŐi�ފ����i0.0~1.0�j���Z�o����
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / length;
	moveRate += speedRate;


	//�X�^�[�g����S�[���܂ł̊Ԃ���b�ԂŐi�ޏꍇ�A�ړ������𔽓]������
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}

	//���`�⊮�ňʒu���Z�o����
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//��]
	oldAngle = { angle.x,angle.y,angle.z };
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;

	//�s��X�V
	UpdateTransform();

	//���C�L���X�g�p�Ƀ��f����Ԃɂ��邽�߂̒P�ʍs���n��
	const DirectX::XMFLOAT4X4 tranceformIdentity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	model->UpdateTransform(tranceformIdentity);
}

//�`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//�\���p�̂��߂̃��[���h�s��ɍX�V����
	model->UpdateTransform(transform);
	shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start,const DirectX::XMFLOAT3& end,HitResult& hit )
{
	//�O��̃��[���h�s��Ƌt�s������߂�
	DirectX::XMMATRIX OldMatrix = DirectX::XMLoadFloat4x4(&OldTransform);
	DirectX::XMMATRIX InverseMatrix = DirectX::XMMatrixInverse(nullptr, OldMatrix);

	//�O��̃��[�J����Ԃł̃��C�ɕϊ�
	DirectX::XMVECTOR startLocal = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&start), InverseMatrix);
	DirectX::XMVECTOR endLocal = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&end), InverseMatrix);


	//���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, startLocal);
	DirectX::XMStoreFloat3(&localEnd, endLocal);
	HitResult localHit;
	if (Collision::InterserctRayVsModel(localStart, localEnd, model, localHit))
	{

		//�O��̏�������̈ʒu�Ɏ������
		DirectX::XMMATRIX WorldTransform;
		WorldTransform = DirectX::XMLoadFloat4x4(&transform);
		//�O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
		//�O�񂩂獡��ɂ����ĕύX���ꂽ���e������Ă���I�u�W�F�N�g�ɔ��f�����
		DirectX::XMVECTOR hitPointLocal = DirectX::XMLoadFloat3(&localHit.position);
		DirectX::XMVECTOR hitPointWorld = DirectX::XMVector3TransformCoord(hitPointLocal, WorldTransform);
		DirectX::XMStoreFloat3(&hit.position, hitPointWorld);

		// �@���x�N�g���̕ϊ�
		DirectX::XMVECTOR normalLocal = DirectX::XMLoadFloat3(&localHit.normal);
		DirectX::XMVECTOR normalWorld = DirectX::XMVector3TransformNormal(normalLocal, WorldTransform);
		DirectX::XMStoreFloat3(&hit.normal, normalWorld);

		//��]�������Z�o
		hit.rotation.x = angle.x - oldAngle.x;
		hit.rotation.y = angle.y - oldAngle.y;
		hit.rotation.z = angle.z - oldAngle.z;

		return true;
	}
	return false;
}
