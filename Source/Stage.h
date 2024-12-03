#pragma once

#include"Graphics/Shader.h"
#include"Collision.h"
#include"Player.h"
#include"SceneManager.h"
#include"SceneTitle.h"

//ステージ
class Stage
{
public:
	Stage(){}
	virtual ~Stage(){}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;
protected:
	void PlayerVsStage()
	{
		Player& player = Player::Instance();
		if (Collision::IntersectSphereVsSphere(
			position,
			radius,
			player.GetPosition(),
			player.GetRadius(),
			DirectX::XMFLOAT3(0, 0, 0)
		))
		{
			SceneManager::instance().ChengeScene(new SceneTitle);
		}
	};
protected:
	struct CollisionMesh
	{
		struct Triangle
		{
			DirectX::XMFLOAT3	positions[3];
			DirectX::XMFLOAT3	normal;
		};

		struct Area
		{
			DirectX::BoundingBox	boundingBox;
			std::vector<int>		triangleIndices;
		};

		std::vector<Triangle>	triangles;
		std::vector<Area>		areas;
	};

	CollisionMesh collisionMesh;

	DirectX::XMFLOAT3 position = { 0,0,0 };
	float radius = 1.0;
};