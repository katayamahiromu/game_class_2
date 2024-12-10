#include"Mask.hlsli"
Texture2D texture0 :register(t0);
SamplerState sampler0 :register(s0);

Texture2D maskTexture : register(t1);

float4 main(VS_OUT pin) :SV_TARGET
{
	float4 color = texture0.Sample(sampler0,pin.texcoord) * pin.color;

	//�}�X�N����ԐF���擾
	float mask = maskTexture.Sample(sampler0, pin.texcoord).r;

	//step�֐���p����mask�̒l��dissolveThreshold�̒l���r���ē��ߒl���Oor�P�ɂ���
	float alpha = step(mask, dissolveThreshold);

	//�΂̏���
	float edgeValue = step(dissolveThreshold, mask)
		* step(mask, dissolveThreshold + edgeThreshold * dissolveThreshold);

	color.rgb += edgeColor.rgb * edgeValue;
	alpha = saturate(alpha + edgeValue);

	//color�̓��ߒl�ɏ�Z
	//color.a *= mask;
	color.a *= alpha;

	//�A���t�@���O�ȉ��Ȃ炻�������`�悵�Ȃ��悤�ɂ���
	clip(color.a - 0.01f);
	return color;
}