#include"Mask.hlsli"
Texture2D texture0 :register(t0);
SamplerState sampler0 :register(s0);

Texture2D maskTexture : register(t1);

float4 main(VS_OUT pin) :SV_TARGET
{
	float4 color = texture0.Sample(sampler0,pin.texcoord) * pin.color;

	//マスクから赤色を取得
	float mask = maskTexture.Sample(sampler0, pin.texcoord).r;

	//step関数を用いてmaskの値とdissolveThresholdの値を比較そて透過値を０or１にする
	float alpha = step(mask, dissolveThreshold);

	//緑の処理
	float edgeValue = step(dissolveThreshold, mask)
		* step(mask, dissolveThreshold + edgeThreshold * dissolveThreshold);

	color.rgb += edgeColor.rgb * edgeValue;
	alpha = saturate(alpha + edgeValue);

	//colorの透過値に乗算
	//color.a *= mask;
	color.a *= alpha;

	//アルファが０以下ならそもそも描画しないようにする
	clip(color.a - 0.01f);
	return color;
}