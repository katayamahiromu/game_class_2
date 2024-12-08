#pragma once
#include<DirectXMath.h>
#include<vector>

struct Switch_info
{
	DirectX::XMFLOAT3 position;
	int type;
};

enum SWITCH_TYPE
{
	HOLD,
	CLICK,
};

struct Cube_info
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	int type;
};

enum CUBE_TYPE
{
	DEFAULT,
	APPEAR,
};

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
	std::vector<Switch_info> Switch_info;
	//どのステージモデルを使うか
	char* path;
	//動かしたいオブジェクトのポジション
	std::vector<DirectX::XMFLOAT3> ObjectPosArray;

	std::vector<Cube_info>Cube_info;
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
	//1
	{
		//プレイヤーの位置
		{-16.035f, 5.233f, 1.502f},
		//ゴールの位置
		{-10.0f, 2.0f, 2.0f},
		//スイッチの位置と種類
		{{{16.0f, 1.3f, 1.502f},CLICK}},
		//使うステージ
		filename[PICTURE_FRAME],
		//動かすオブジェクト
		{{13.0f,16.0f,5.0f}},
		//ブロックの位置、拡大率、種類
		{}
	},

	//2
	{
		{-16.035f, 5.233f, 1.502f},
		{16.0f, 2.0f, 2.0f},
		{{{8.0f, 1.3f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{-10.035f, 5.233f, 1.502f}},
		{}
	},

	//3
	{
		{-16.035f, 5.233f, 1.502f},
		{0.0f, 2.0f, 2.0f},
		{{{-10.0f, 1.3f, 1.502f},HOLD},{{10.0f, 1.3f, 1.502f},CLICK}},
		filename[PICTURE_FRAME],
		{{0.0f, 5.233f, 1.502f}},
		{}
	},

	//4
	{
		{-16.035f, 5.233f, 1.502f},
		{10.0f, 2.0f, 2.0f},
		{{{0.0f, 5.5f, 1.502f},CLICK}},
		filename[PICTURE_FRAME],
		{{-2.0f,16.0f,1.5f}},
		{{{ 0.0f, 5.0f, 1.5f },{10.0f,1.0f,1.0f},APPEAR}}
	},

	//5
	{
		{-16.035f, 10.233f, 1.502f},
		{14.018f, 7.0f, 1.502f},
		{{{-5.0f, 1.3f, 1.502f},CLICK},{{12.0f, 6.3f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{{{ -11.0f, 4.0f, 1.5f },{10.5f,5.0f,1.0f},DEFAULT},{{ 6.3f, 4.0f, 1.5f },{20.5f,5.0f,1.0f},DEFAULT}}
	},

	//6
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},


	//7
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//8
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//9
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//10
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//11
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//12
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//13
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//14
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},

	//15
	{
		{16.035f, 5.233f, 1.502f},
		{-16.018f, 2.229f, 1.502f},
		{{{0.0f, 6.0f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{}
	},
};