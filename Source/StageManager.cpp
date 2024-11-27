#include"StageManager.h"
#include"Goal.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
	//更新処理
	for (Stage* stage : stages) {
		stage->Update(elapsedTime);
	}
	for (GameObject* object : objects)
	{
		object->Update(elapsedTime);
	}

	//破棄処理
	for (Stage* stage : stageRemoves)
	{
		std::vector<Stage*>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end())
		{
			stages.erase(it);
		}
		//ステージの破棄
		delete stage;
	}
	for (GameObject* object : objectRemoves)
	{
		std::vector<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), object);
		if (it != objects.end())
		{
			objects.erase(it);
		}
		//ステージの破棄
		delete object;
	}

	//Update内で増えた奴を登録
	for (Stage* stage_add : stageAdd)
	{
		StageRegister(stage_add);
	}
	for (GameObject* o_add : objectAdd)
	{
		ObjectRegister(o_add);
	}

	//破棄リストをクリア
	stageRemoves.clear();
	objectRemoves.clear();
	//追加リストをクリア
	stageAdd.clear();
	objectRemoves.clear();
}

//描画処理
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

//ステージとレイキャスト
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

//ステージ登録
void StageManager::StageRegister(Stage* stage)
{
	stages.emplace_back(stage);
}

void StageManager::ObjectRegister(GameObject* r_object)
{
	objects.emplace_back(r_object);
}

//ステージ全削除
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


//ステージ削除
void StageManager::StageRemove(Stage*stage)
{
	stageRemoves.insert(stage);
}

void StageManager::ObjectRemove(GameObject* r_object)
{
	objectRemoves.insert(r_object);
}
