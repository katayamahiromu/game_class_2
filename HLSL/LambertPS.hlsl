#include "Lambert.hlsli"
#include"FilterFunctions.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); //�V���h�E�}�b�v

SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//�@���}�b�v����xyz�������擾���āi-1 ~ +1�j�̊ԂɃX�P�[�����O
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//�ϊ��p��3X3�s���p�ӂ���
	float3x3 CM =
	{
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal)
	};

	// �@���}�b�v�Ŏ擾�����@�����ɕϊ��s����|�����킹,���K������
	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(-lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//�}�e���A���萔
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	//�����̌v�Z
	float3 ambient = ka * ambientLightColor;

	//���s�����̃��C�e�B���O�v�Z
	float3 directionalDiffuse = ClacHarfLambert(N, L, ambientLightColor.rgb, kd);
	float3 directionalSpecular = CalcPhongSpecular(N, L, ambientLightColor.rgb, E, shiness, ks);

	color.rgb += color.rgb * directionalDiffuse;
	color.rgb += directionalSpecular;


	// RGB > HSV�ɕϊ�
	//�J���[�O�����f�B���O�̏���
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