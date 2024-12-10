#include"Goal.h"
#include"Player.h"
#include"Collision.h"
#include"SceneManager.h"
#include"SceneStageSelect.h"
#include"StageManager.h"

Goal::Goal(DirectX::XMFLOAT3 Position)
{
	position.x = Position.x;
	position.y = Position.y - 1.0f;
	position.z = Position.z;
	model = std::make_unique<Model>("Data/Model/Door/Door.mdl");
	scale.x = scale.y = scale.z = 0.002f;
}

Goal::~Goal()
{

}

void Goal::Update(float elapsedTime)
{
	UpdateTransform();
	model->UpdateTransform(transform);
	PlayerVsGoal();
}

void Goal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
		debugRenderer->DrawSphere({position.x,position.y + 1.0f,position.z}, radius, DirectX::XMFLOAT4(1, 1, 1, 1));
	}
	shader->Draw(dc, model.get());
}

bool Goal::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return false;
}

void Goal::PlayerVsGoal()
{
	StageManager& manger = StageManager::Instance();
	if (manger.GetGoalCount() == manger.GetPushCount())
	{
		Player& player = Player::Instance();
		if (Collision::IntersectSphereVsSphere(
			{position.x,position.y + 1.0f,position.z},
			radius,
			player.GetPosition(),
			player.GetRadius(),
			DirectX::XMFLOAT3(0, 0, 0)
		))
		{
			SceneManager::instance().ChengeScene(new SceneStageSelect);
		}
	}
}