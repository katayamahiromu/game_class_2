#include "Lambert.hlsli"
#include"FilterFunctions.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//カラーグランディングの処理
	// RGB > HSVに変換
	color.rgb = RGB2HSV(color.rgb);

	//色相調整
	color.r += hueShift;

	//彩度調整
	color.g *= saturation;

	//明度調整
	color.b *= brightness;

	//HSV > RGBに変換
	color.rgb = HSV2RGB(color.rgb);
	return color;
}