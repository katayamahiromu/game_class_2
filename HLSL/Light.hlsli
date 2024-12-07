//--------------------------------------------
//	�n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//--------------------------------------------
// normal		: �@��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// kd			: ���˗�(���˂̋���)
float3 ClacHarfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = saturate(dot(normal, -lightVector) * 0.5 + 0.5);
	return lightColor * d * kd;
}

//--------------------------------------------
//	�t�H���̋��ʔ��ˌv�Z�֐�
//--------------------------------------------
// normal		: �@��(���K���ς�)
// lightVector	: ���˃x�N�g��(���K���ς�)
// lightColor	: ���ˌ��F
// eyeVector	: �����x�N�g��(���K���ς�)
// shininess	: ����x(���������l)
// ks			: ���˗�(���˂̋���)
// �Ԃ��l		: ���ʔ��ːF
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
	float3 eveVector, float shininess, float3 ks)
{
	//���˃x�N�g����@�������ɑ΂��Ĕ��˂������x�N�g�����Z�o
	float3 R = reflect(lightVector, normal);

	//���˃x�N�g���Ǝ����x�N�g���œ���
	//���ς������ʂ��O�ȉ��ł���΂O�ɂ���
	float d = max(dot(eveVector, R), 0);

	//���򏈗�
	//���˂͕\�ʂ��c���c���Ȃقǃn�C���C�g�͋����Ȃ�A�͈͂�������
	//�t�ɕ\�ʂ��U���U���ȂقǁA�n�C���C�g�͎キ�Ȃ�A�͈͍͂L����
	//���̕ω��͎w���I�ɕω����邽�ߌ���x���搔�Ƃ��Ă��悤

	d = pow(d, shininess);

	//���ˌ��F�Ɠ��ς̌��ʁA�y�є��˗���S�ď�Z���ĕԋp���܂��傤
	return lightColor * d * ks;
}