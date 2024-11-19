#pragma once
#include"Stage.h"
#include"Graphics/Model.h"
#include"Character.h"

class Switch : public Stage,Character
{
public:
	Switch(/*DirectX::XMFLOAT3 Position*/);
	~Switch()override;

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

	//�ʒu�ݒ�
	void SetPositon(const DirectX::XMFLOAT3& position) { this->Character::position = position; }
private:
	//���̓G�l�~�[�ł���Ă܂�����ŕύX���܂�
	void SwitchVsEnemy();
	bool IsGoal = false;
};