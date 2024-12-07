#include"switch.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"StageManager.h"

ClickSwitch::ClickSwitch(DirectX::XMFLOAT3 Position)
{
	model = std::make_unique<Model>("Data/Model/switch/swtich.mdl");
	position = Position;
	scale.x = scale.y = scale.z = 0.01f;
	//�O�̂���
	UpdateTranceform();
	model->UpdateTransform(transform);
}

ClickSwitch::~ClickSwitch()
{

}

void ClickSwitch::Update(float elapsedTime)
{
	UpdateTranceform();
	model->UpdateTransform(transform);
	SwitchVsEnemy();
	OnSwitchMove();
}

void ClickSwitch::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model.get());
}

bool ClickSwitch::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return false;
}

void ClickSwitch::OnSwitchMove()
{
	if (pushFlag == true)
	{
		scale.y *= 0.9;
		if (scale.y < 0.0001f)
		{
			scale.y = 0.0001f;
		}
	}
}

void ClickSwitch::SwitchVsEnemy()
{
	if (pushFlag == true) return;
	//�X�e�[�W�ƃI�u�W�F�N�g�̂����蔻��
	StageManager& stageManager = StageManager::Instance();
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
			stageManager.PushCountPlus();
			pushFlag = true;
		}
	}
}

HoldSwitch::HoldSwitch(DirectX::XMFLOAT3 Position)
{

	model = std::make_unique<Model>("Data/Model/switch/swtich.mdl");
	position = Position;
	scale.x = scale.y = scale.z = 0.01f;
	//�O�̂���
	UpdateTranceform();
	model->UpdateTransform(transform);
}

HoldSwitch::~HoldSwitch()
{

}

void HoldSwitch::Update(float elapsedTime)
{
	UpdateTranceform();
	model->UpdateTransform(transform);
	SwitchVsEnemy();
	OnSwitchMove();
}

void HoldSwitch::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model.get());
}

bool HoldSwitch::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return false;
}

void HoldSwitch::SwitchVsEnemy()
{
	StageManager& stageManager = StageManager::Instance();
	//�������Ă�����
	if (pushFlag == true)
	{
		//���񓖂����ĂȂ��������ɂ���
		stageManager.PushCountMinus();
		pushFlag = false;
	}

	//�X�C�b�`�ƃI�u�W�F�N�g�̂����蔻��
	for (int i = 0;i < EnemeyManager::Instance().GetEnemyCount();++i)
	{
		Enemy* enemy = EnemeyManager::Instance().GetEnemy(i);
		if (Collision::IntersectSphereVsSphere(
			position,
			radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			DirectX::XMFLOAT3(0, 0, 0)
		))
		{
			stageManager.PushCountPlus();
			pushFlag = true;
		}
	}
}

void HoldSwitch::OnSwitchMove()
{
	if (pushFlag == true)
	{
		scale.y *= 0.9;
		if (scale.y < 0.0001f)
		{
			scale.y = 0.0001f;
		}
	}
	else
	{
		scale.y = 0.01;
	}
}