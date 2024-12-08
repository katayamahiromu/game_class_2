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
/// �X�e�[�W�̏��
/// </summary>
/// <param name="PlayerPos">: �v���C���[�̃|�W�V����</param>
/// <param name="GoalPos">: �S�[���̃|�W�V����</param>
/// <param name="SwitchPos">: �X�C�b�`�̃|�W�V����</param>
/// <param name="pushSwitchCount">: �S�[�����邽�߂̉����X�C�b�`�̐�</param>
/// <param name="path">: �g�������X�e�[�W�f�[�^</param>
struct StageScript
{
	DirectX::XMFLOAT3 PlayerPos;
	DirectX::XMFLOAT3 GoalPos;
	std::vector<Switch_info> Switch_info;
	//�ǂ̃X�e�[�W���f�����g����
	char* path;
	//�����������I�u�W�F�N�g�̃|�W�V����
	std::vector<DirectX::XMFLOAT3> ObjectPosArray;

	std::vector<Cube_info>Cube_info;
};

//�X�e�[�W�̃t�@�C���p�X
char* filename[] =
{
	"Data/Model/ExampleStage/�z��.mdl",
	"Data/Model/ExampleStage/ExampleStage.mdl",
	"Data/Model/ExampleStage/�X�e�[�W2.mdl",
};

//�g�������X�e�[�W
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
		//�v���C���[�̈ʒu
		{-16.035f, 5.233f, 1.502f},
		//�S�[���̈ʒu
		{-10.0f, 2.0f, 2.0f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{{{16.0f, 1.3f, 1.502f},CLICK}},
		//�g���X�e�[�W
		filename[PICTURE_FRAME],
		//�������I�u�W�F�N�g
		{{13.0f,16.0f,5.0f}},
		//�u���b�N�̈ʒu�A�g�嗦�A���
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