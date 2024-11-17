#include"switch.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"StageManager.h"
#include"Goal.h"

Switch::Switch(DirectX::XMFLOAT3 Position)
{
	model = std::make_unique<Model>("Data/Model/switch/swtich.mdl");
	//ただの設計ミスです後で直します
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
			//押した感を出すために毎フレーム小さくしていく
			scale.y *= 0.9;

			//ゴールを出す
			if (!IsGoal)
			{
				DirectX::XMFLOAT3 pos = { -16.018f, 2.229f, 1.502f };
				Goal* goal = new Goal(pos);
				StageManager::Instance().RegisterAdd(goal);
				IsGoal = true;
			}
		}
		else
		{
			scale.y = 0.01f;
			if (IsGoal)
			{
				std::vector<Stage*>Array= StageManager::Instance().GetArray();
				StageManager::Instance().Remove(Array.at(Array.size() - 1));
				IsGoal = false;
			}
		}
	}
}