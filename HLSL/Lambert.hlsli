#include"Light.hlsli"

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 world_position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer CbScene : register(b0)
{
	row_major float4x4	viewProjection;
	float4				viewPosition;
	float4				lightDirection;
	float4				ambientLightColor;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};

cbuffer CBColorGrading : register(b3)
{
	float hueShift; //êFëäí≤êÆ
	float saturation; //ç ìxí≤êÆ
	float brightness; //ñæìxí≤êÆ
	float dummy;
};