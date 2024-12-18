#include"StageManager.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
	//更新処理
	for (Stage* stage : stages) {
		stage->Update(elapsedTime);
	}

	//破棄処理
	for (Stage* stage : removes)
	{
		std::vector<Stage*>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end())
		{
			stages.erase(it);
		}
		//ステージの破棄
		delete stage;
	}

	//Update内で増えた奴を登録
	for (Stage* stage_add : add)
	{
		Register(stage_add);
	}
	//破棄リストをクリア
	removes.clear();
	//追加リストをクリア
	add.clear();
}

//描画処理
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Stage* stage : stages) {
		stage->Render(dc, shader);
	}
}

//ステージ登録
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}

//ステージ全削除
void StageManager::Clear()
{
	for (Stage* stage : stages)
	{
		delete stage;
	}
	stages.clear();
}

//レイキャスト
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

//ステージ削除
void StageManager::Remove(Stage*stage)
{
	removes.insert(stage);
}

//配列の追加
void StageManager::RegisterAdd(Stage* stage)
{
	add.push_back(stage);
}