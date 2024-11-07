#include"ProjectileStraight.h"
#include"StageManager.h"

//�R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	model = new Model("Data/Model/Sword/Sword.mdl");
	//�\���T�C�Y�𒲐�
	//scale.x = scale.y = scale.z = 0.5f;
	scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
ProjectileStraight::~ProjectileStraight() {
	delete model;
}

//�X�V����
void ProjectileStraight::Update(float elapsedTime)
{
	//��������
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0) {
		//�������폜
		Destroy();
	}
	//�����ړ�����
	UpdateHorizontalMove(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model->UpdateTransform(transform);
}

//�`�揈��
void ProjectileStraight::Render(ID3D11DeviceContext*dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}

void ProjectileStraight::UpdateHorizontalMove(float elapsedTime)
{
	float speed = this->speed * elapsedTime;
	float mx = direction.x * speed;
	float mz = direction.z * speed;

	DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
	DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };
	HitResult hit;
	if (StageManager::Instance().RayCast(start,end,hit)) {

		//���˃x�N�g��
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

		// ���˃x�N�g���̌v�Z
		DirectX::XMVECTOR Reflected = DirectX::XMVectorSubtract(Vec, DirectX::XMVectorScale(Normal, 2.0f * distance));

		//�@���̑傫���������ɍ��킹��
		//this->direction = DirectX::XMFLOAT3(DirectX::XMVectorGetX(Reflected), 0.0f, DirectX::XMVectorGetZ(Reflected));
		DirectX::XMStoreFloat3(&direction, Reflected);
	}
	else
	{
		//�ړ�
		position.x += mx;
		position.z += mz;
	}
}