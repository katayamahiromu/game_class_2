#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

//�F���␳���
struct ColorGradingData
{
	float hueShift = 0;// �F������
	float saturation = 1;// �ʓx����
	float brightness = 1;// ���x����
	float dummy = 0;
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;

	ColorGradingData colorGradingData;
};