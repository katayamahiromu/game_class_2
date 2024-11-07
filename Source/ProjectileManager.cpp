#include"ProjectileManager.h"
#include"Graphics/Graphics.h"
//�R���X�g���N�^
ProjectileManager::ProjectileManager() {
}

//�f�X�g���N�^
ProjectileManager::~ProjectileManager() {
	Clear();
}
//�X�V����
void ProjectileManager::Update(float elapsedTime) {
	//�X�V����
	for (auto& projectiles : projectiles) {
		projectiles->Update(elapsedTime);
	}
	//�j������
	//*Projectiles�͈̔�for������erase()����ƕs����������Ă��܂�����
	//�X�V�������I��������Ƃɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g��j������
	for (Projectile* projectile : remove) {
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[���g��Ȃ���΂Ȃ�Ȃ�
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end()) {
			projectiles.erase(it);
		}
		//�e�ۂ̔j������
		delete projectile;
	}
	//�j�����X�g���N���A
	remove.clear();
}

//�`�揈��
void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* sheder) {
	for (auto& projectiles : projectiles) {
		projectiles->Render(dc, sheder);
	}
}

//�f�o�b�N�v���~�e�B�u�`��
void ProjectileManager::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	for (auto& projectiles : projectiles) {
		debugRenderer->DrawSphere(projectiles->GetPosition(),0.5, DirectX::XMFLOAT4(0, 0, 0, 1));
	}
}

//�e�ۓo�^
void ProjectileManager::Register(Projectile* projectile) {
	projectiles.push_back(projectile);
}

//�e�ۑS�폜
void ProjectileManager::Clear() {
	projectiles.clear();
}

//�e�ۍ폜
void ProjectileManager::Remove(Projectile* projectile) {
	//�j�����X�g�ɒǉ�
	remove.insert(projectile);
}