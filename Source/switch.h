#pragma once
#include"Stage.h"
#include"Graphics/Model.h"
#include"Character.h"

class Switch : public Stage
{
public:
	Switch(DirectX::XMFLOAT3 Position,int t);
	~Switch()override;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
private:
	enum Switch_type
	{
		Hold,
		Click
	};
private:
	//���̓G�l�~�[�ł���Ă܂�����ŕύX���܂�
	void SwitchVsEnemy();

	//�X�P�[���ő��삵�Ă�̂Œl�����͏����ʓ|������
	float downSpeed = 0.9f;

	//�X�C�b�`�̃^�C�v
	int type;
};