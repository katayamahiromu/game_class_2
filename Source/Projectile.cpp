#include"Projectile.h"
#include"ProjectileManager.h"
#include"Graphics/Graphics.h"

//�R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager):manager(manager)
{
	manager->Register(this);
}


//�f�o�b�N�v���~�e�B�u�`��
void Projectile::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	debugRenderer->DrawSphere(this->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}
//�s��X�V����
void Projectile::UpdateTransform() {
	DirectX::XMVECTOR Front, Up, Right;
	
	//�O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);
	//���̏�x�N�g�����Z�o
	Up = { 0,1,0,0 };
	//�E�x�N�g�����Z�o
	Right = DirectX::XMVector3Cross(Front, Up);
	Right = DirectX::XMVector3Normalize(Right);
	//��x�N�g�����Z�o
	Up = DirectX::XMVector3Cross(Right, Front);
	Up = DirectX::XMVector3Normalize(Up);

	//�v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//�Z�o�������x�N�g������s����쐬
	transform._11 = scale.x * right.x;
	transform._12 = scale.x * right.y;
	transform._13 = scale.x * right.z;
	transform._14 = 0.0f;
	transform._21 = scale.y * up.x;
	transform._22 = scale.y * up.y;
	transform._23 = scale.y * up.z;
	transform._24 = 0.0f;
	transform._31 = scale.z * front.x;
	transform._32 = scale.z * front.y;
	transform._33 = scale.z * front.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	//���˕���
	this->direction = front;
}

//�j��
void Projectile::Destroy() {
	manager->Remove(this);
}

//����
void Projectile::reflection()
{

}