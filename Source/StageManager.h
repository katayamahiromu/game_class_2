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

	void SetGoalCount(int count) { goalCount = count; }
	int GetGoalCount() { return goalCount; }
	void PushCountPlus() { pushCount++; }
	void ClearPushCount() { pushCount = 0; }
	int GetPushCount() { return pushCount; }
	void SetGoalPosition(DirectX::XMFLOAT3 pos) { goalPosition = pos; }
private:
	std::vector<Stage*>stages;
	std::set<Stage*>removes;
	std::vector<Stage*>add;

	//ゴールを出すためのカウント
	int goalCount = 0;
	//今押されてるスイッチのカウント
	int pushCount = 0;
	DirectX::XMFLOAT3 goalPosition = {0,0,0};
};