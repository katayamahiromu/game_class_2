#pragma once
#include"Scene.h"

//シーンマネージャー
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	//唯一のインスタンス取得
	static SceneManager& instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render();
	//シーンクリア
	void Clear();
	//シーン切り替え
	void ChengeScene(Scene* scene);


	int GetSelect() { return select; }
	void SetSelect(int se) { select = se; }

	void ResetIsGoal() { isGoal = false; }
	void CanGoal() { isGoal = true; }
	bool GetIsGoal() { return isGoal; }
private:
	int select = 0;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	bool isGoal = false;
};

