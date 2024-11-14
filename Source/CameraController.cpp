#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"
#include<cmath>

//�X�V����
void CameraController::Update(float elapsedTime) 
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	////�J�����̉�]���x
	//float speed = rollspeed * elapsedTime;

	////�X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
	//angle.x += ay * speed;
	//angle.y += ax * speed;

	////X���̃J������]�𐧌�
	//if (angle.x > maxAngleX) 
	//{
	//	angle.x = maxAngleX;
	//}
	//if (angle.x < mixAngleX)
	//{
	//	angle.x = mixAngleX;
	//}

	////Y���̉�]�l��-3.14 ~ 3.14�Ɏ��܂�悤�ɂ���
	//if (angle.y < -DirectX::XM_PI)
	//{
	//	angle.y += DirectX::XM_2PI;
	//}
	//if (angle.y > DirectX::XM_PI)
	//{
	//	angle.y -= DirectX::XM_2PI;
	//}

	////�΂�
	//float spring = 10.0;
	////����
	//float damping = 2.0f;

	
	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}


//void CameraController::TPS_Update(float elapsedTime)
//{
//	GamePad& gamePad = Input::Instance().GetGamePad();
//	float ax = gamePad.GetAxisRX();
//	float ay = gamePad.GetAxisRY();
//	//�J�����̉�]���x
//	float speed = rollspeed * elapsedTime;
//
//	//�X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
//	angle.x += ay * speed;
//	angle.y += ax * speed;
//
//	//X���̃J������]�𐧌�
//	if (angle.x > maxAngleX)
//	{
//		angle.x = maxAngleX;
//	}
//	if (angle.x < mixAngleX)
//	{
//		angle.x = mixAngleX;
//	}
//
//	//Y���̉�]�l��-3.14 ~ 3.14�Ɏ��܂�悤�ɂ���
//	if (angle.y < -DirectX::XM_PI)
//	{
//		angle.y += DirectX::XM_2PI;
//	}
//	if (angle.y > DirectX::XM_PI)
//	{
//		angle.y -= DirectX::XM_2PI;
//	}
//
//
//	//�J������]�l����]�s��ɕϊ�
//	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//
//	//��]�s�񂩂�O�����x�N�g�������o��
//	DirectX::XMVECTOR Front = Transform.r[2];
//	DirectX::XMFLOAT3 front;
//	DirectX::XMStoreFloat3(&front, Front);
//
//	float spring = 10.0;
//	float damping = 2.0;
//
//	float CalculateDamping = spring / (2 * std::sqrtf(spring));
//
//	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
//	DirectX::XMFLOAT3 eye;
//	eye.x = CalculateDamping * target.x - front.x * range;
//	eye.y = CalculateDamping * target.y - front.y * range;
//	eye.z = CalculateDamping * target.z - front.z * range;
//
//	//�J�����̎��_�ƒ����_��ݒ�
//	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
//}