#pragma once

#include"Graphics/Model.h"
#include"Projectile.h"

//�����e��
class ProjectileStraight :public Projectile 
{
public:
	ProjectileStraight(ProjectileManager*manager);
	~ProjectileStraight() override;

	//�X�V����
	void Update(float elapsedTime) override;
	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* sheder) override;
	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
	//�����ړ�
	void UpdateHorizontalMove(float elapsedTime);
private:
	Model* model = nullptr;
	float speed = 10.0f;
	float lifeTimer = 3.0f;
};