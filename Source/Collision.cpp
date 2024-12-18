#include"Collision.h"

//球と球の交差判定
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB
)
{
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec		= DirectX::XMVectorSubtract(PositionA, PositionB);
	DirectX::XMVECTOR LenthSq	= DirectX::XMVector3LengthSq(Vec);
	float lenthSq;
	DirectX::XMStoreFloat(&lenthSq, LenthSq);

	//距離判定
	float range = lenthSq - (radiusA + radiusB);
	if (range >= 0.0f)
	{
		return false;
	}

	//AがBを押し出す
	Vec = DirectX::XMVectorScale(Vec, range);//調整
	DirectX::XMVECTOR add = DirectX::XMVectorAdd(PositionB, Vec);
	DirectX::XMStoreFloat3(&outPositionB, add);
	return true;
}

bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
) 
{
	if (positionA.y > positionB.y + heightB) {
		return false;
	}
	if (positionA.y + heightA < positionB.y) {
		return false;
	}
	//XZ平面での範囲チェック
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float dis = sqrtf(vx * vx + vz * vz);
	float range = radiusA + radiusB;
	if (range < dis) {
		return false;
	}

	//AがBを押し出す
	vx = vx / dis * range;
	vz = vz / dis * range;
	outPositionB.x = positionA.x + vx;
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + vz;
	return true;
}

////トップビュー状態の円柱と円柱の交差判定
//bool Collision::IntersectCylinderVsCylinderTopView(
//	const DirectX::XMFLOAT3& positionA, 
//	float radiusA, 
//	float heightA, 
//	const DirectX::XMFLOAT3& positionB, 
//	float radiusB, 
//	float heightB, 
//	DirectX::XMFLOAT3& outPositionB)
//{
//	if (positionA.z > positionB.z + heightB) {
//		return false;
//	}
//	if (positionA.z + heightA < positionB.z) {
//		return false;
//	}
//	//XY平面での範囲チェック
//	float vx = positionB.x - positionA.x;
//	float vy = positionB.y - positionA.y;
//	float dis = sqrtf(vx * vx + vy * vy);
//	float range = radiusA + radiusB;
//	if (range < dis) {
//		return false;
//	}
//
//	//AがBを押し出す
//	vx = vx / dis * range;
//	vy = vy / dis * range;
//	outPositionB.x = positionA.x + vx;
//	outPositionB.y = positionB.y + vy;
//	outPositionB.z = positionA.z;
//	return true;
//}

bool  Collision::IntersctSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//高さチェック
	if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;

	//XZ平面での範囲チェック
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	//球が円柱を押し出す
	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	return true;
}

//四角形と四角形の交差判定
bool Collision::IntersectCubeVsCube(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
)
{
	DirectX::BoundingBox a;
	//Boxの中心
	a.Center = positionA;
	//中心から範囲
	a.Extents = { radiusA,heightA,radiusA };

	DirectX::BoundingBox b;
	b.Center = positionB;
	b.Extents = { radiusB,heightB,radiusB };

	if (a.Intersects(b))
	{
		// 各軸の重なり量を計算
		float overlapX = (a.Extents.x + b.Extents.x) - std::abs(a.Center.x - b.Center.x);
		float overlapY = (a.Extents.y + b.Extents.y) - std::abs(a.Center.y - b.Center.y);
		float overlapZ = (a.Extents.z + b.Extents.z) - std::abs(a.Center.z - b.Center.z);

		//球と違ってどれかの軸のみを移動するため重なりが一番少ない軸を選択
		DirectX::XMFLOAT3 correctionVector = { 0.0f, 0.0f, 0.0f };
		if (overlapX < overlapY && overlapX < overlapZ)
		{
			// X軸方向に押し出し
			correctionVector.x = (a.Center.x < b.Center.x ? overlapX : -overlapX);
		}
		else if (overlapY < overlapZ)
		{
			// Y軸方向に押し出し
			correctionVector.y = (a.Center.y < b.Center.y ? overlapY : -overlapY);
		}
		else
		{
			// Z軸方向に押し出し
			correctionVector.z = (a.Center.z < b.Center.z ? overlapZ : -overlapZ);
		}

		// Bの位置を修正
		outPositionB.x = positionB.x + correctionVector.x;
		outPositionB.y = positionB.y + correctionVector.y;
		outPositionB.z = positionB.z + correctionVector.z;

		return true; // 衝突した
	}
	return false;
}

//レイとモデルの交差判定
bool Collision::InterserctRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result
) 
{
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	//ワールド空間のレイの長さ
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//メッシュノード取得
		const Model::Node& nodo = model->GetNodes().at(mesh.nodeIndex);

		//レイをワールド空間からローカル空間へ変数
		DirectX::XMMATRIX WorldTranceform = DirectX::XMLoadFloat4x4(&nodo.worldTransform);
		DirectX::XMMATRIX InverseWorldTranceform = DirectX::XMMatrixInverse(nullptr, WorldTranceform);
		
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTranceform);
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTranceform);
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//三角形（面）との交差判定
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0;i < subset.indexCount;i += 3)
			{
				UINT index = subset.startIndex + i;
				//三角形の頂点を抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index+2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//三角形の三辺ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//三角形の法線ベクトルを算出
				DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, BC));

				//内積の結果がプラスなら表向き
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V,N);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0)continue;

				//レイと平面の交差を算出
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
				float x;
				DirectX::XMStoreFloat(&x, X);
				//交差までの距離が今までに計算した最近距離より
				//大きい時はスキップ
				if (x < 0.0f || x > neart)continue;
				DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

				//交差が三角形の内側にあるか判定
				//1つ目
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);

				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0) continue;
				//２つ目
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);

				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0)continue;
				//３つ目
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);

				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0)continue;

				//最新距離を更新
				neart = x;
				//交点と法線を更新
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}

		if (materialIndex >= 0)
		{
			//ローカル空間からワールド空間への変換
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTranceform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//ヒット情報保存
			if(result.distance>distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTranceform);
				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}
	}
	return hit;
}