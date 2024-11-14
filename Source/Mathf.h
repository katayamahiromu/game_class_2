#pragma once
#include<DirectXMath.h>

//���������Z�p
class Mathf
{
public:
	//���`�⊮
	static float Leap(float a, float b, float t);
	//�w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);
	//���`�⊮
	static DirectX::XMFLOAT3 Leap(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float t);
};
