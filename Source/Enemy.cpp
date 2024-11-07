#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManeger.h"

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�Փ˗p����p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�j��
void Enemy::Destroy() {
	EnemeyManager::Instance().Remove(this);
}