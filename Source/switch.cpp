#include"switch.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"StageManager.h"

Switch::Switch(DirectX::XMFLOAT3 Position,int t)
{
	model = std::make_unique<Model>("Data/Model/switch/swtich.mdl");
	position = Position;
	type = t;
	scale.x = scale.y = scale.z = 0.01f;
	//�O�̂���
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
	StageManager& stageManager = StageManager::Instance();
	//���t���[���N���A
	if(type == Hold) stageManager.ClearPushCount();
	for (int i = 0;i < EnemeyManager::Instance().GetEnemyCount();++i)
	{
		Enemy* enemy = EnemeyManager::Instance().GetEnemy(i);
		if (Collision::IntersectSphereVsSphere(
			position,
			radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			DirectX::XMFLOAT3(0,0,0)
		))
		{
			//�����������o�����߂ɖ��t���[�����������Ă���
			scale.y *= downSpeed;
			//������Ă���J�E���g���グ��
			stageManager.PushCountPlus();
		}
		else
		{
			scale.y = 0.01f;
		}
	}
}