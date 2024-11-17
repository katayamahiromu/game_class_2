#include"switch.h"
#include"EnemyManeger.h"
#include"Collision.h"

Switch::Switch(DirectX::XMFLOAT3 Position)
{
	model = std::make_unique<Model>("Data/Model/switch/swtich.mdl");
	//�����̐݌v�~�X�ł���Œ����܂�
	Character::position = Position;
	scale.x = scale.y = scale.z = 0.01f;
	UpdateTranceform();
	model->UpdateTransform(transform);
}

Switch::~Switch()
{

}

void Switch::Update(float elapsedTime)
{
	UpdateTranceform();
	model->UpdateTransform(transform);
	SwitchVsEnemy();
}

void Switch::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model.get());
}

bool Switch::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return false;
}

void Switch::SwitchVsEnemy()
{
	for (int i = 0;i < EnemeyManager::Instance().GetEnemyCount();++i)
	{
		Enemy* enemy = EnemeyManager::Instance().GetEnemy(i);
		if (Collision::IntersectSphereVsSphere(
			Character::position,
			Character::radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			DirectX::XMFLOAT3(0,0,0)
		))
		{
			scale.y *= 0.9;
		}
		else
		{
			scale.y = 0.01f;
		}
	}
}