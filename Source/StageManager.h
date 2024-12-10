#pragma once

#include<vector>
#include<set>
#include"Stage.h"

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

	//レイキャスト
	bool RaycastToStage(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	//ステージorオブジェクト登録
	void StageRegister(Stage* stage);
	void ObjectRegister(GameObject* r_object);

	//ステージorオブジェクト削除
	void StageRemove(Stage* stage);
	void ObjectRemove(GameObject* r_object);

	//ステージ全削除
	void Clear();

	//ボタン関係の関数共
	int GetPushCount() { return pushCount; }
	void PushCountPlus() { pushCount++; }
	void PushCountMinus() { pushCount--; }
	void ClearPushCount() { pushCount = 0; }
	void ResetSwitch();

	//ゴール関係の関数共
	int GetGoalCount() { return goalCount; }
	void SetGoalCount(int count) { goalCount = count; }
	void SetGoalPosition(DirectX::XMFLOAT3 pos) { goalPosition = pos; }
private:
	std::vector<Stage*>stages;
	std::vector<GameObject*>objects;
	std::set<Stage*>stageRemoves;
	std::set<GameObject*>objectRemoves;
	std::set<Stage*>stageAdd;
	std::set<GameObject*>objectAdd;

	//ゴールを出すためのカウント
	int goalCount = 0;
	//今押されてるスイッチのカウント
	int pushCount = 0;
	DirectX::XMFLOAT3 goalPosition = {0,0,0};
};