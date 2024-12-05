#include "Lambert.hlsli"
#include"FilterFunctions.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); //シャドウマップ

SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//法線マップからxyz成分を取得して（-1 ~ +1）の間にスケーリング
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//変換用の3X3行列を用意する
	float3x3 CM =
	{
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal)
	};

	// 法線マップで取得した法線情報に変換行列を掛け合わせ,正規化する
	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(-lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//マテリアル定数
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	//環境光の計算
	float3 ambient = ka * ambientLightColor;

	//平行光源のライティング計算
	float3 directionalDiffuse = ClacHarfLambert(N, L, ambientLightColor.rgb, kd);
	float3 directionalSpecular = CalcPhongSpecular(N, L, ambientLightColor.rgb, E, shiness, ks);

	color.rgb += color.rgb * directionalDiffuse;
	color.rgb += directionalSpecular;


	// RGB > HSVに変換
	//カラーグランディングの処理
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