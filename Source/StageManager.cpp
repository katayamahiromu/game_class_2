#include"StageManager.h"
#include"Goal.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
	//�X�V����
	for (Stage* stage : stages) {
		stage->Update(elapsedTime);
	}
	for (GameObject* object : objects)
	{
		object->Update(elapsedTime);
	}

	//�j������
	for (Stage* stage : stageRemoves)
	{
		std::vector<Stage*>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end())
		{
			stages.erase(it);
		}
		//�X�e�[�W�̔j��
		delete stage;
	}
	for (GameObject* object : objectRemoves)
	{
		std::vector<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), object);
		if (it != objects.end())
		{
			objects.erase(it);
		}
		//�X�e�[�W�̔j��
		delete object;
	}

	//Update���ő������z��o�^
	for (Stage* stage_add : stageAdd)
	{
		StageRegister(stage_add);
	}
	for (GameObject* o_add : objectAdd)
	{
		ObjectRegister(o_add);
	}

	//�j�����X�g���N���A
	stageRemoves.clear();
	objectRemoves.clear();
	//�ǉ����X�g���N���A
	stageAdd.clear();
	objectRemoves.clear();
}

//�`�揈��
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (GameObject* stage : stages) {
		stage->Render(dc, shader);
	}
	for (GameObject* object : objects)
	{
		object->Render(dc, shader);
	}
}

//�X�e�[�W�ƃ��C�L���X�g
bool StageManager::RaycastToStage(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	for (Stage* stage : stages)
	{
		if (stage->RayCast(start, end, hit))
		{
			return true;
		}
	}
	return false;
}

//�X�e�[�W�o�^
void StageManager::StageRegister(Stage* stage)
{
	stages.emplace_back(stage);
}

void StageManager::ObjectRegister(GameObject* r_object)
{
	objects.emplace_back(r_object);
}

//�X�e�[�W�S�폜
void StageManager::Clear()
{
	for (GameObject* stage : stages)
	{
		delete stage;
	}
	stages.clear();

	for (GameObject* object : objects)
	{
		delete object;
	}
	objects.clear();
}


//�X�e�[�W�폜
void StageManager::StageRemove(Stage*stage)
{
	stageRemoves.insert(stage);
}

void StageManager::ObjectRemove(GameObject* r_object)
{
	objectRemoves.insert(r_object);
}
