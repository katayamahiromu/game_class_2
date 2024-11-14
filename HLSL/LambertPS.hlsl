#include "Lambert.hlsli"
#include"FilterFunctions.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//�J���[�O�����f�B���O�̏���
	// RGB > HSV�ɕϊ�
	color.rgb = RGB2HSV(color.rgb);

	//�F������
	color.r += hueShift;

	//�ʓx����
	color.g *= saturation;

	//���x����
	color.b *= brightness;

	//HSV > RGB�ɕϊ�
	color.rgb = HSV2RGB(color.rgb);
	return color;
}