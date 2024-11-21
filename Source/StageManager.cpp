#include"StageManager.h"
#include"Goal.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
	//�X�V����
	for (Stage* stage : stages) {
		stage->Update(elapsedTime);
	}

	//�j������
	for (Stage* stage : removes)
	{
		std::vector<Stage*>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end())
		{
			stages.erase(it);
		}
		//�X�e�[�W�̔j��
		delete stage;
	}

	//Update���ő������z��o�^
	for (Stage* stage_add : add)
	{
		Register(stage_add);
	}
	//�j�����X�g���N���A
	removes.clear();
	//�ǉ����X�g���N���A
	add.clear();

	//�S�[�����o�����̐���
	AppearGoal();
}

//�`�揈��
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Stage* stage : stages) {
		stage->Render(dc, shader);
	}
}

//�X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}

//�X�e�[�W�S�폜
void StageManager::Clear()
{
	for (Stage* stage : stages)
	{
		delete stage;
	}
	stages.clear();
}

//���C�L���X�g
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	for (Stage* stage : stages)
	{
		if (stage->RayCast(
			start,
			end,
			hit
		)) {
			return result = true;
		}
	}
	return result;
}

//�X�e�[�W�폜
void StageManager::Remove(Stage*stage)
{
	removes.insert(stage);
}

//�z��̒ǉ�
void StageManager::RegisterAdd(Stage* stage)
{
	add.push_back(stage);
}

void StageManager::AppearGoal()
{
	if (pushCount == goalCount)
	{
		//���ɃS�[�����������珈�����Ȃ�
		if (IsGoal)return;
		Register(new Goal(goalPosition));
		IsGoal = true;
	}
	else
	{
		if (IsGoal)
		{
			Remove(stages.at(stages.size() - 1));
		}
		IsGoal = false;
	}
}