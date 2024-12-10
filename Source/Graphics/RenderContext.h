#pragma once
#include<d3d11.h>
#include <DirectXMath.h>


//�}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
	float dissolveThreshold;
	float edgThreshold; //�΂�臒l
	DirectX::XMFLOAT4 edgColor; //�΂̐F
};

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
	//�f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* deviceContext;

	//�}�X�N�f�[�^
	MaskData maskData;

	//�J����
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//���s�����p
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		ambientColor;


	ColorGradingData colorGradingData;
};