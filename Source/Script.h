#pragma once
#include<DirectXMath.h>

/// <summary>
/// �X�e�[�W�̏��
/// </summary>
/// <param name="PlayerPos">�v���C���[�̃|�W�V����</param>
/// <param name="GoalPos">�S�[���̃|�W�V����</param>
/// <param name="SwitchPos">�X�C�b�`�̃|�W�V����</param>
/// <param name="pushSwitchCount">�S�[�����邽�߂̉����X�C�b�`�̐�</param>
/// <param name="path">�g�������X�e�[�W�f�[�^</param>
struct StageScript
{
	DirectX::XMFLOAT3 PlayerPos;
	DirectX::XMFLOAT3 GoalPos;
	DirectX::XMFLOAT3 SwitchPos;
	//�X�C�b�`������������S�[����������
	int pushSwitchCount;
	//�ǂ̃X�e�[�W���f�����g����
	char* path;
};

//�X�e�[�W�̃t�@�C���p�X
char* filename[] =
{
	"Data/Model/ExampleStage/�z��.mdl",
	"Data/Model/ExampleStage/ExampleStage.mdl",
	"Data/Model/ExampleStage/�X�e�[�W2.mdl",
};

//�g�������X�e�[�W
enum 
{
	PICTURE_FRAME = 0,
	EXAMPLE,
	STAGETWO,
};

StageScript script[] =
{
	//1~5
	{{16.035f, 5.233f, 1.502f},{-16.018f, 2.229f, 1.502f},{10.0f, 1.3f, 1.502f},1,filename[PICTURE_FRAME]},
	{{16.035f, 5.233f, 1.502f},{-16.018f, 2.229f, 1.502f},{10.0f, 1.3f, 1.502f},1,filename[EXAMPLE]},
};