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
	"Data/Model/Slime/Slime.mdl",
};

//�g�������X�e�[�W
enum STAGE_NUM
{
	PICTURE_FRAME = 0,
	EXAMPLE,
	STAGETWO,
	STAGENULL,
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
		{16.018f, 7.0f, 1.502f},
		{{{-5.0f, 1.3f, 1.502f},CLICK},{{12.0f, 6.5f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{{{ -11.0f, 4.0f, 1.5f },{10.5f,5.0f,1.0f},DEFAULT},{{ 6.3f, 4.0f, 1.5f },{20.5f,5.0f,1.0f},DEFAULT}}
	},

	//6
	{
		{-16.035f, 5.233f, 1.502f},
		{11.5f, 2.0f, 1.5f},
		{{{-9.0f,12.0f,1.5f},CLICK},{{-2.6f, 6.8f, 1.5f},CLICK},{{ 3.3f, 12.5f, 1.5f},CLICK},{{11.5f, 6.0f, 1.5f},HOLD}},
		filename[PICTURE_FRAME],
		{{-11.0f,13.0f,5.0f}},
		{{{ -9.0f, 11.5f, 1.5f },{5.8f,1.0f,1.0f},DEFAULT},{{ -2.6f, 6.3f, 1.5f },{7.1f,1.0f,1.0f},DEFAULT},{{ 3.3f, 12.0f, 1.5f },{6.7f,1.0f,1.0f},DEFAULT},{{ 11.5f, 5.5f, 1.5f },{8.0f,1.0f,1.0f},APPEAR}}
	},


	//7
	{
		{-16.035f, 10.233f, 1.502f},
		{ 14.0f, 2.229f, 1.5f},
		{{{-11.0f, 6.5f, 1.5f},CLICK},{{-2.6f, 1.3f, 1.5f},HOLD},{{14.0f, 6.8f, 1.5f},HOLD}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f},{-12.0f, 6.5f, 1.5f},{13.0f, 6.8f, 1.5f}},
		{{{ -11.0f, 4.0f, 1.5f },{10.3f,5.0f,1.0f},DEFAULT},{{ -2.6f, 8.0f, 1.5f },{6.7f,3.2f,1.0f},DEFAULT},{{ 13.0f, 6.3f, 1.5f },{5.0f,1.0f,1.0f},APPEAR}}
	},

	//8
	{
		{0.0f, 10.0f, 1.502f},
		{0.0f, 9.229f, 1.502f},
		{{{16.0f, 1.3f, 1.502f},CLICK},{{-16.0f, 1.3f, 1.502f},CLICK},{{16.0f, 16.5f, 1.502f},CLICK},{{-16.0f, 16.5f, 1.502f},CLICK}},
		filename[PICTURE_FRAME],
		{{2.0f,10.0f,5.0f}},
		{{{ -5.8f, 7.0f, 1.5f },{20.0f,2.3f,1.0f},DEFAULT},{{ 5.8f, 7.0f, 1.5f },{20.0f,2.3f,1.0f},DEFAULT}}
	},

	//9
	{
		{-16.035f, 10.233f, 1.502f},
		{16.018f, 8.2f, 1.502f},
		{{{-4.0f, 1.3f, 1.502f},HOLD},{{4.0f, 1.3f, 1.502f},CLICK}},
		filename[PICTURE_FRAME],
		{{-10.0f,10.0f,5.0f}},
		{{{ -11.0f, 4.3f, 1.5f },{12.0f,5.8f,1.0f},DEFAULT},{{ 0.3f, 4.3f, 1.5f },{5.4f,5.8f,1.0f},DEFAULT},{{ 11.0f, 4.3f, 1.5f },{11.2f,5.8f,1.0f},DEFAULT}}
	},

	//10
	{
		{-16.035f, 5.233f, 1.502f},
		{12.018f, 1.5f, 1.502f},
		{{{12.0f, 7.5f, 1.502f},HOLD}},
		filename[PICTURE_FRAME],
		{{8.0f,10.0f,5.0f}},
		{{{ 10.0f, 7.0f, 1.5f },{13.0f,1.0f,1.0f},APPEAR}}
	},

	
	//11
	{
		//�v���C���[�̈ʒu
		{7.487f, 11.291f, 1.502f},
		//�S�[���̈ʒu
		{15.511f, 11.291f, 1.502f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{
			{{11.1f, 10.4f, 1.502f},HOLD},
			{{-15.208f, 1.233f, 1.502f},CLICK}
		},
		//�g���X�e�[�W
		filename[PICTURE_FRAME],
		//�������I�u�W�F�N�g
		{{-11.27f,13.177f,5.0f}},
		//�u���b�N�̈ʒu�A�g�嗦�A���
		{
			{{ -10.8f, 12.28f, 1.502f }, {11.6f,1.0f,1.0f},DEFAULT},
			{{ -10.8f, 11.28f, 1.502f }, {11.6f,1.0f,1.0f},DEFAULT},
			{{ 14.512f,10.791f,1.502f}, {4.535f,1.0f,1.0f},DEFAULT},
			{{   10.8f, 9.791f,1.502f},  {12.5f,1.0f,1.0f},DEFAULT},
			{{   7.35f,10.791f,1.502f}, {5.535f,1.0f,1.0f},DEFAULT},
			{{  1.116f, 1.748f,1.502f},{30.053f,1.0f,1.0f},DEFAULT},
		}
	},

	//12
	{
		//�v���C���[�̈ʒu
		{6.778f, 2.785f, 1.502f},
		//�S�[���̈ʒu
		{9.813f, 13.282f, 1.502f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{
			{{-15.383f, 1.233f, 1.502f},CLICK},
			{{ 15.383f, 1.233f, 1.502f},HOLD},
		},
		//�g���X�e�[�W
		filename[PICTURE_FRAME],
		//�������I�u�W�F�N�g
		{
			{   0.0f, 3.748f,1.502f},
			{-9.754f,13.282f,1.502f},
		},
		//�u���b�N�̈ʒu�A�g�嗦�A���
		{
			{{ -9.302f, 12.279f, 1.502f },{15.512f,1.0f,1.0f},DEFAULT},
			{{  9.302f, 12.279f, 1.502f },{15.512f,1.0f,1.0f},DEFAULT},
			{{  0.0f, 1.748f, 1.502f}, {29.0f,1.0f,1.0f},DEFAULT},
			{{  0.0f, 2.748f, 1.502f}, {1.0f,2.0f,1.0f},DEFAULT},
		}
	},

	//13
	{
		//�v���C���[�̈ʒu
		{4.033f, 7.849f, 1.502f},
		//�S�[���̈ʒu
		{2.033f, 7.849f, 1.502f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{
			{{-16.0f, 2.248f, 1.502f},HOLD},
			{{ 11.5f,13.523f, 1.502f},HOLD},
			{{ 0.717f,4.849f, 1.502f},CLICK},
		},
		//�g���X�e�[�W
		filename[PICTURE_FRAME],
		//�������I�u�W�F�N�g
		{
			{  -6.9f,  7.57f,1.502f},
			{-4.309f,10.547f,1.502f},
			{11.897f, 8.965f,1.502f},
		},
		//�u���b�N�̈ʒu�A�g�嗦�A���
		{
			{{ 12.279f,  4.837f, 1.502f }, {9.163f, 8.256f,1.0f},DEFAULT},
			{{ 11.535f, 13.023f, 1.502f }, {10.977f,  1.0f,1.0f},DEFAULT},
			{{ -4.093f,  4.349f, 1.502f }, { 2.814f,  1.0f,1.0f},DEFAULT},

			{{  2.605f,  4.349f, 1.502f }, { 5.535f,  1.0f,1.0f},DEFAULT},
			{{  3.349f,  5.349f, 1.502f }, { 3.9f,    1.0f,1.0f},DEFAULT},
			{{  3.349f,  6.349f, 1.502f }, { 3.9f,    1.0f,1.0f},DEFAULT},
			{{  2.605f,  7.349f, 1.502f }, { 5.535f,  1.0f,1.0f},DEFAULT},
			{{  3.349f,  5.581f, 1.502f }, { 3.721f,3.721f,1.0f},DEFAULT},

			{{    0.0f,  1.748f, 1.502f }, { 35.0f,   1.0f,1.0f},DEFAULT},
			{{ -4.093f, 10.047f, 1.502f }, { 5.442f,  1.0f,1.0f},DEFAULT},

			//{{-12.279f,  10.791f,  1.502f },{  1.0f,13.884f,1.0f},DEFAULT},
			//{{-13.279f,  10.791f,  1.502f },{  1.0f,13.884f,1.0f},DEFAULT},
			{{-14.279f,  10.791f,  1.502f },{  5.0f,13.884f,1.0f},DEFAULT},
			//{{-15.279f,  10.791f,  1.502f },{  1.0f,13.884f,1.0f},DEFAULT},
			//{{-16.279f,  10.791f,  1.502f },{  1.0f,13.884f,1.0f},DEFAULT},

			{{ -6.326f,  7.070f,  1.502f },{7.349f,   1.0f,1.0f},DEFAULT},
			{{ -8.558f,  6.070f,  1.502f },{2.814f,   1.0f,1.0f},DEFAULT},
			{{ -8.558f,  5.070f,  1.502f },{2.814f,   1.0f,1.0f},DEFAULT},
			{{ -8.558f,  6.070f,  1.502f },{2.814f,   3.0f,1.0f},DEFAULT},
		}
	},

	//14
	{
		//�v���C���[�̈ʒu
		{-2.949f, 12.233f, 1.502f},
		//�S�[���̈ʒu
		{-8.001f, 12.233f, 1.502f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{
			{{-7.293f, 3.913f, 1.502f},CLICK},
			{{ 9.588f,13.722f, 1.502f},CLICK},
			{{-6.654f,12.233f, 1.502f},HOLD},
		},
		//�g���X�e�[�W
		filename[PICTURE_FRAME],
		//�������I�u�W�F�N�g
		{{-4.764f,12.233f,1.502f}},
		//�u���b�N�̈ʒu�A�g�嗦�A���
		{
			{{   15.5f,  9.302f, 1.502f },{  2.0f,17.326f,1.0f},DEFAULT},
			{{  -15.0f,  9.302f, 1.502f },{  3.0f,17.326f,1.0f},DEFAULT},
			{{ 14.267f, 11.779f, 1.502f },{  2.0f,11.000f,1.0f},DEFAULT},
			{{  6.326f, 10.047f, 1.502f },{7.349f, 7.349f,1.0f},DEFAULT},
			{{ -4.837f,  8.558f, 1.502f },{7.349f, 7.349f,1.0f},DEFAULT},
		}
	},

	//15
	{
		//�v���C���[�̈ʒu
		{9.173f, 12.802f, 1.502f},
		//�S�[���̈ʒu
		{1.86f, 7.57f, 1.502f},
		//�X�C�b�`�̈ʒu�Ǝ��
		{{{-15.0f, 2.36f, 1.502f},CLICK}},
		//�g���X�e�[�W
		filename[STAGENULL],
		//�������I�u�W�F�N�g
		{{9.173f,9.802f,1.502f}},
		//�u���b�N�̈ʒu�A�g�嗦�A���
		{
			{{ -0.372f, 1.860f, 1.502f },  {33.744f,   1.0f,1.0f},APPEAR},
			{{  0.372f,  16.0f, 1.502f },  {33.744f,   1.0f,1.0f},APPEAR},
			{{  -16.0f, 9.302f, 1.502f },  {   1.0f,  15.0f,1.0f},APPEAR},
			{{   16.0f, 9.302f, 1.502f },  {   1.0f,  15.0f,1.0f},APPEAR},

			{{  1.116f, 2.360f, 1.502f },  {29.116f,   2.0f,1.0f},APPEAR},
			{{-13.200f, 7.070f, 1.502f },  {   2.0f,11.884f,1.0f},APPEAR},
			{{ -4.837f,  12.1f, 1.502f },  {17.326f, 1.907f,1.0f},APPEAR},
			{{ -4.837f,  12.1f, 1.502f },  {17.326f, 1.907f,1.0f},APPEAR},
			{{  3.349f,   9.8f, 1.502f },  {   1.0f, 6.442f,1.0f},APPEAR},
			{{ -1.860f, 7.070f, 1.502f },  {10.977f,   1.0f,1.0f},APPEAR},

			{{ 13.023f, 5.581f, 1.502f },  { 6.442f, 8.256f,1.0f},APPEAR},
			{{  9.302f, 9.302f, 1.502f },  { 4.628f,   1.0f,1.0f},APPEAR},
			{{  9.302f, 12.302f,1.502f },  { 4.628f,   1.0f,1.0f},APPEAR},

			{{  4.837f,  3.349f,1.500f },  {   1.0f, 2.814f,1.0f},APPEAR},
		}
	},
};