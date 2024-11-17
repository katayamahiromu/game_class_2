#pragma once

#include<vector>
#include"Stage.h"
#include<set>

//ステージマネージャー
class StageManager
{
private:
	StageManager() {};
	~StageManager() {};
public:
	//唯一のインスタンス取得
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//ステージ登録
	void Register(Stage* stage);

	//ステージ削除
	void Remove(Stage* stage);

	//ステージ全削除
	void Clear();

	//レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	//配列の追加
	void RegisterAdd(Stage* stage);

	//後で消す♡ 配列を取得
	std::vector<Stage*> GetArray() { return stages; }
private:
	std::vector<Stage*>stages;
	std::set<Stage*>removes;
	std::vector<Stage*>add;
};