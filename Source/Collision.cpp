#include"Collision.h"

//���Ƌ��̌�������
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

	//��������
	float range = lenthSq - (radiusA + radiusB);
	if (range >= 0.0f)
	{
		return false;
	}

	//A��B�������o��
	Vec = DirectX::XMVectorScale(Vec, range);//����
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
	//XZ���ʂł͈̔̓`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float dis = sqrtf(vx * vx + vz * vz);
	float range = radiusA + radiusB;
	if (range < dis) {
		return false;
	}

	//A��B�������o��
	vx = vx / dis * range;
	vz = vz / dis * range;
	outPositionB.x = positionA.x + vx;
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + vz;
	return true;
}

bool  Collision::IntersctSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//�����`�F�b�N
	if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	//�����~���������o��
	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	return true;
}

//���C�ƃ��f���̌�������
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

	//���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//���b�V���m�[�h�擾
		const Model::Node& nodo = model->GetNodes().at(mesh.nodeIndex);

		//���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϐ�
		DirectX::XMMATRIX WorldTranceform = DirectX::XMLoadFloat4x4(&nodo.worldTransform);
		DirectX::XMMATRIX InverseWorldTranceform = DirectX::XMMatrixInverse(nullptr, WorldTranceform);
		
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTranceform);
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTranceform);
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//�O�p�`�i�ʁj�Ƃ̌�������
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
				//�O�p�`�̒��_�𒊏o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index+1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index+2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//�O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(AB, BC));

				//���ς̌��ʂ��v���X�Ȃ�\����
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V,N);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0)continue;

				//���C�ƕ��ʂ̌������Z�o
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
				float x;
				DirectX::XMStoreFloat(&x, X);
				//�����܂ł̋��������܂łɌv�Z�����ŋߋ������
				//�傫�����̓X�L�b�v
				if (x < 0.0f || x > neart)continue;
				DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

				//�������O�p�`�̓����ɂ��邩����
				//1��
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);

				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0) continue;
				//�Q��
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);

				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0)continue;
				//�R��
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);

				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0)continue;

				//�ŐV�������X�V
				neart = x;
				//��_�Ɩ@�����X�V
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}

		if (materialIndex >= 0)
		{
			//���[�J����Ԃ��烏�[���h��Ԃւ̕ϊ�
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTranceform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//�q�b�g���ۑ�
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