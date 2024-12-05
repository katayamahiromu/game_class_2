#pragma once
#include<DirectXMath.h>
#include<vector>

/// <summary>
/// ステージの情報
/// </summary>
/// <param name="PlayerPos">: プレイヤーのポジション</param>
/// <param name="GoalPos">: ゴールのポジション</param>
/// <param name="SwitchPos">: スイッチのポジション</param>
/// <param name="pushSwitchCount">: ゴールするための押すスイッチの数</param>
/// <param name="path">: 使いたいステージデータ</param>
struct StageScript
{
	DirectX::XMFLOAT3 PlayerPos;
	DirectX::XMFLOAT3 GoalPos;
	std::vector<DirectX::XMFLOAT3> SwitchPosArray;
	//どのステージモデルを使うか
	char* path;
	//動かしたいオブジェクトのポジション
	std::vector<DirectX::XMFLOAT3> ObjectPosArray;
};

//ステージのファイルパス
char* filename[] =
{
	"Data/Model/ExampleStage/額縁.mdl",
	"Data/Model/ExampleStage/ExampleStage.mdl",
	"Data/Model/ExampleStage/ステージ2.mdl",
};

//使いたいステージ
enum STAGE_NUM
{
	PICTURE_FRAME = 0,
	EXAMPLE,
	STAGETWO,
};

const StageScript script[] =
{
	//1~5
	{{16.035f, 5.233f, 1.502f},{16.018f, 2.229f, 1.502f},{{-16.0f, 1.3f, 1.502f}},filename[PICTURE_FRAME],{{2.0f,10.0f,5.0f}}},
	{{16.035f, 5.233f, 1.502f},{-16.018f, 2.229f, 1.502f},{{10.0f, 1.3f, 1.502f}},filename[STAGETWO],{{2.0f,10.0f,5.0f}}},
	{{16.035f, 5.233f, 1.502f},{-16.018f, 2.229f, 1.502f},{{10.0f, 1.3f, 1.502f}},filename[EXAMPLE],{{2.0f,10.0f,5.0f}}},
};