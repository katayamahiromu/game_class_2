#pragma once
#include<DirectXMath.h>
#include <DirectXCollision.h>
#include"Graphics/Model.h"

//ヒットエフェクト
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交差
	DirectX::XMFLOAT3 normal = { 0,0,0 };//衝突したポリゴンの法線ベクトル
	float distance = 0.0f;//レイの視点から交差までの距離
	int materialIndex = -1;//衝突したポリゴンのマテリアル番号
	DirectX::XMFLOAT3 rotation = { 0,0,0 }; //回転量
};

//コリジョン
class Collision
{
public:
	//球と球の交差判定
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	//円柱と円柱の交差判定
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	//四角形と四角形の交差判定
	static bool IntersectCubeVsCube(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	////トップビュー状態の円柱と円柱の交差判定
	//static bool IntersectCylinderVsCylinderTopView(
	//	const DirectX::XMFLOAT3& positionA,
	//	float radiusA,
	//	float heightA,
	//	const DirectX::XMFLOAT3& positionB,
	//	float radiusB,
	//	float heightB,
	//	DirectX::XMFLOAT3& outPositionB
	//);

	//球と円柱の交差判定
	static bool IntersctSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition);
	//レイとモデルの交差判定
	static bool InterserctRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);
};
