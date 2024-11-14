#pragma once
#include<DirectXMath.h>
#include"Graphics/Model.h"
#include<memory>

//�L�����N�^�[
class Character
{
public:
	Character() {}
	virtual ~Character() {}
	void UpdateTranceform();

	//�ʒu���擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	//�ʒu�ݒ�
	void SetPositon(const DirectX::XMFLOAT3& position) { this->position = position; }
	//��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	//��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	//�X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	//���a�擾
	float GetRadius() const { return radius; }
	//�ړ�����
	void Move(float vx, float vz, float speed);
	//���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);
	//�W�����v����
	void Junp(float speed);
	//���͏����X�V
	void UpdateVelocity(float elapsedTime);
	//�n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }
	//�����擾
	float GetHeight() const { return height; }
	//�_���[�W��^����
	bool ApplyDamage(int damage,float invincibleTime);
	//�Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);
	//���N��Ԃ��擾
	int GetHealth() const { return health; }
	//�ő匒�N��Ԃ��擾
	int GetMaxhealth() const { return maxHealth; }

	Model* GetModel() { return model.get(); }

	bool isXYMode = false; // XY�ړ����[�h�t���O

	void ToggleMoveMode() { isXYMode = !isXYMode; } // �ړ����[�h�̐؂�ւ�


	void SetTransform(DirectX::XMFLOAT4X4 trans) { transform = trans; }
	DirectX::XMFLOAT4X4& GetTransform() {return transform; }

protected:
	DirectX::XMFLOAT3   position = { 0,0,0 };
	DirectX::XMFLOAT3   angle = { 0,0,0 };
	DirectX::XMFLOAT3   scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	float radius  =  0.5;
	float gravity = -1.0f;
	DirectX::XMFLOAT3 velocity = { 0,0,0 };
	//���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding() {};
	float height  =  2.0f;

	//�_���[�W���󂯂����ɌĂ΂��
	virtual void OnDamaged(){}
	//���S�������ɌĂ΂��
	virtual void OnDead(){}
	//���G���ԍX�V
	void UpdateInvinciblTImer(float elapsedTime);
private:
	//�������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);
	//�����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);
	//�������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);
	//�����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);

protected:
	bool isGround = false;
	int health = 5;
	int maxHealth = 5;
	float invicibleTime = 1.0f;
	float friction = 0.5f;
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;
	float airControl = 0.3f;
	float stepOffset = 1.0f;
	float slopeRate = 1.0f;

	struct AABB
	{
		DirectX::XMFLOAT3 max;
		DirectX::XMFLOAT3 min;
	};

	std::unique_ptr<Model>model;
};