#pragma once
#include "Object.h"
#include"Graphics/Model.h"
#include"Character.h"
#include "Graphics/Shader.h"
#include "Collision.h"

class ClickSwitch : public GameObject
{
public:
	ClickSwitch(DirectX::XMFLOAT3 Position);
	~ClickSwitch()override;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
private:
	//�X�C�b�`�������ꂽ���̓���
	void OnSwitchMove();
	//���̓G�l�~�[�ł���Ă܂�����ŕύX���܂�
	void SwitchVsEnemy();

	//�X�P�[���ő��삵�Ă�̂Œl�����͏����ʓ|������
	float downSpeed = 0.9f;

	//�����ꂽ�t���O
	bool pushFlag = false;
};


class HoldSwitch : public GameObject
{
public:
	HoldSwitch(DirectX::XMFLOAT3 Position);
	~HoldSwitch()override;

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
private:
	//�X�C�b�`�������ꂽ���̓���
	void OnSwitchMove();
	//���̓G�l�~�[�ł���Ă܂�����ŕύX���܂�
	void SwitchVsEnemy();

	//�X�P�[���ő��삵�Ă�̂Œl�����͏����ʓ|������
	float downSpeed = 0.9f;

	//�����ꂽ�t���O
	bool pushFlag = false;
};