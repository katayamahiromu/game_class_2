#pragma once
#include<DirectXMath.h>

//�J�����R���g�����[
class CameraController
{
public:
	CameraController(){}
	~CameraController(){}

	//�X�V����
	void Update(float elapsedTime);

	//�^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	//TPS�p�̃J����
	//void TPS_Update(float elapsedTiem);
private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle  = { 0,DirectX::XMConvertToRadians(180),0};
	float rollspeed = DirectX::XMConvertToRadians(90);
	float range     = 25.0f;

	float maxAngleX = DirectX::XMConvertToRadians(45);
	float mixAngleX = DirectX::XMConvertToRadians(-45);

};