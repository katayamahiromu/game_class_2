#pragma once
#include "Object.h"
#include"Graphics/Model.h"
#include"Character.h"
#include "Graphics/Shader.h"
#include "Collision.h"

class Switch : public GameObject
{
public:
	Switch(DirectX::XMFLOAT3 Position);
	~Switch();

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);
private:
	//���̓G�l�~�[�ł���Ă܂�����ŕύX���܂�
	void SwitchVsEnemy();
	//�X�P�[���ő��삵�Ă�̂Œl�����͏����ʓ|������
	float downSpeed = 0.9f;

	//�����蔻��p�̔��a
	float radius = 0.5f;
};