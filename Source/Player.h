#pragma once

#include"Graphics/Shader.h"
#include"Character.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"Effect.h"

//�v���C���[
class Player :public Character
{
public:
	// �^�C�g���ŕ\������p�̃R���X�g���N�^
	Player();

	Player(DirectX::XMFLOAT3 pos);
	~Player() override;

	//�X�V����
	void Update(float elapsedTime);

	// �^�C�g���p�̍X�V����(�قڃA�j���[�V�����Đ�����)
	void TitleUpdate(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�pGUI�`��
	void DrawDebugGui();
	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	//�ړ����͏���
	bool InputMove(float elapsedTime);
	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	//�v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	void CollisionPlayerVsEnemies();
	//�W�����v���͏���
	bool InputJump();
	//���n�������ɌĂ΂��
	void OnLanding()override;
	//�U�����͏���
	bool InputAttack();
	//�m�[�h�ƃG�l�~�[�̏Փˏ���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);
	//�C���X�^���X�擾
	static Player& Instance();
protected:
	//�_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;
	//���S�������ɌĂ΂��
	void OnDead()override;
private:
	//�ҋ@�X�e�[�g�֑J��
	void TransitiomIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdelState(float elapsedTime);

	//�ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	//�ړ��X�e�[�g�X�V����
	void UpdateMoveState(float elapseTime);

	//�W�����v�X�e�[�g�֑J��
	void TransitionJumpState();

	//�W�����v�X�e�[�g�X�V����
	void UpdateJumpState(float elapsedTime);

	//���n�X�e�[�g�֑J��
	void TransitionLandState();

	//���n�X�e�[�g�X�V����
	void UpdateLandState(float elapsedTime);

	//�U���X�e�[�g�֑J��
	void TransitionAttackState();

	//�U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

	//�_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	//�_���[�W�X�e�[�g�X�V����
	void UpdateDamageState(float elapsedTime);

	//���S�X�e�[�g�֑J��
	void TransitionDeathState();

	//���S�X�e�[�g�X�V����
	void UpdateDeathState(float elapsedTime);

	//�����X�e�[�g�֑J��
	void TransitionReviveState();

	//�����X�e�[�g�X�V����
	void UpdateReviveState(float elapsedTime);
private:
	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float JumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	Effect* hitEffect = nullptr;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	//�A�j���[�V����
	enum Animation
	{
		Anime_Attack,
		Anime_Death,
		Anime_Falling,
		Anime_GetHit1,
		Anime_GetHit2,
		Anime_Idle,
		Anime_Jump,
		Anime_Jump_Flip,
		Anime_Landing,
		Anime_Revive,
		Anime_Running,
		Anime_Walking
	};

	//�X�e�[�g
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive
	};
private:
	State state = State::Idle;
public:
	//���Z�b�g�̎��ɋL�^���Ă����̂����ɂɖ߂��ׂ�public
	//�^��̏���
	void InitRecording();
private:
	//�^��
	void Recording(DirectX::XMFLOAT3 position);
	//�Đ�
	void Playback(Character*character);

	//static const int MAX_KEEP_TRANSFORM = 720;
	//static const int MAX_KEEP_TRANSFORM = 432;
	static const int MAX_KEEP_TRANSFORM = 1440;
	DirectX::XMFLOAT3 keep_position[MAX_KEEP_TRANSFORM];
	//�^��t���O
	bool IsRecording = false;
	//�Đ��t���O
	bool IsPlayback = false;
	//�ŐV�̏��͂O�ɓ����Ă���
	int playback_count = 0;

	//���������ĂȂ�
	DirectX::XMFLOAT3 ENOUGTH = {FLT_MAX,FLT_MAX,FLT_MAX};
};