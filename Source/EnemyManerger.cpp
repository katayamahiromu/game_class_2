#include"EnemyManeger.h"
#include"Graphics/Graphics.h"
#include"Collision.h"

//�X�V����
void EnemeyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies) {
		enemy->Update(elapsedTime);
	}
    //�j������
    //�X�V�������I��������Ƃɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g��j������
    for (Enemy* enemy : removes) {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[���g��Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end()) {
            enemies.erase(it);
        }
        //�G�l�~�[�̔j������
        delete enemy;
    }
    //�j�����X�g���N���A
    removes.clear();
	//�G���m�̏Փ˔���
	CollisionEnemyVsEnemy();
}

//�`�揈��
void EnemeyManager::Render(ID3D11DeviceContext* dc, Shader* shader) {
	for (Enemy* enemy : enemies) {
		enemy->Render(dc, shader);
	}
}

//�G�l�~�[�o�^
void EnemeyManager::Register(Enemy* enemy) {
	enemies.emplace_back(enemy);
}

//�G�l�~�[�폜
void EnemeyManager::Remove(Enemy* enemy){
    //�j�����X�g�ɒǉ�
    removes.insert(enemy);
}

//�G�l�~�[�S�폜
void EnemeyManager::Clear() {
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemeyManager::DrawDebugPrimitive() {
	for (Enemy* enemy : enemies) {
		enemy->DrawDebugPrimitive();
	}
}

void EnemeyManager::CollisionEnemyVsEnemy() {
    int enemyCount = GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy2 = GetEnemy(j);
            if (enemy1 == enemy2)continue;

            // �Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder
            (enemy1->GetPosition(),
             enemy1->GetRadius(),
             enemy1->GetHeight(),
             enemy2->GetPosition(),
             enemy2->GetRadius(),
             enemy2->GetHeight(),
             outPosition))
            {
                // �����o����̈ʒu����
                enemy2->SetPositon(outPosition);
            }
        }
    }
}