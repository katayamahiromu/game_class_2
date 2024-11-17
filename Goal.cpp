#include"Goal.h"
#include"Player.h"
#include"Collision.h"
#include"SceneManager.h"
#include"SceneTitle.h"

Goal::Goal(DirectX::XMFLOAT3 Position)
{
	position = Position;
}

Goal::~Goal()
{

}

void Goal::Update(float elapsedTime)
{
	PlayerVsGoal();
}

void Goal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	debugRenderer->DrawSphere(position, radius,DirectX::XMFLOAT4(1,1,1,1));
}

void Goal::PlayerVsGoal()
{
	Player& player = Player::Instance();
	if (Collision::IntersectSphereVsSphere(
		position,
		radius,
		player.GetPosition(),
		player.GetRadius(),
		DirectX::XMFLOAT3(0,0,0)
	))
	{
		SceneManager::instance().ChengeScene(new SceneTitle);
	}
}

bool Goal::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return false;
}