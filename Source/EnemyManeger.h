#pragma once
#include<vector>
#include<set>
#include"Enemy.h"

class EnemeyManager
{
private:
	EnemeyManager(){}
	~EnemeyManager(){}
public:
	static EnemeyManager& Instance()
	{
		static EnemeyManager instance;
		return instance;
	}
	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	//�G�l�~�[�o�^
	void StageRegister(Enemy* enemy);
	//�G�l�~�[�폜
	void Remove(Enemy* enemy);
	//�G�l�~�[�S�폜
	void Clear();
	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	//�G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
	//�G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }
	//�G�l�~�[���m�̏Փ˔���
	void CollisionEnemyVsEnemy();
private:
	std::vector<Enemy*> enemies;
	std::set<Enemy*>stageRemoves;
};