#pragma once
#include "../CharacterBase/CharacterBase.h"
#include"../VariableParam.h"
#include"../../StateMachine/StateAI.h"
#include<memory>
#include"../Camera.h"

class WireRope;

enum class PlayerState {

	StayState,//����s�\���

	IdleState,//�ʏ���
	
	JumpState,//�W�����v
	
	HangWallState,//�͂ݏ��
	HangDropState,//�͂݉���
	ClimeState,//�Ǐ��

	WireAimState,//���C���[�ő_���X�e�[�g
	WireShotState,//���C���[������
	WireActionState,//���[�v�ɕ߂܂�
	
	ArrowAimState,//�|��˃X�e�[�g
	
	DamageState,//�_���[�W
	DeathState,//�͐s����
};


class Player : public CharacterBase , public ICamera{
public:

	static const char* move_speed;
	static const char* jump_power;
	static const char* height;
	static const char* arm_range;


	enum PlayerAnimJam {
		Idle = 15,
		Jump = 22,
		HangWall = 13,
		Clime = 4,
		HangDrop = 11,
		WalkBack = 26,
		WalkForward = 27,
		WalkLeft = 28,
		WalkRight = 29,
		RunBack = 20,
		RunForward = 21,
		RunLeft = 23,
		RunRight = 24,
		AimReady = 19,
		AimIdle = 2,
		Shot = 3,
		WireAction = 10,
		Damage = 5,
		Death = 8,
	};

private:

	static Player* mp_Instance;

public:
	static Player* GetInstance();

	CModelObj m_bow;

	CollisionTask m_Col;

	CollisionTask m_AttackCol;

	StateAI<Player, PlayerState> m_StateAI;

	float m_NextAng;
	//�Ǔo��p�̐��R���C�_�[(���E2��)
	CLine m_LineCol[2];
	//�W�����v�p�̐��R���C�_�[
	CLine m_JumpLine;
public:
	CVector3D m_OldPos;

	CVector3D m_ClimePoint;
	//�͂񂾃��[�v�̃|�C���^�[
	WireRope* mp_WireRope;

	//��-�v�̐ڑ��_
	CVector3D m_RopeConectPoint;

	//�J�o�|���Ă���Ƃ��̔w�̖ʂ̖@��
	CVector3D m_CoverWallNormal;

	VariableParam<float> m_Param;
	VariableParam<bool> m_Flag;

	int m_RandID;

	float m_HitPoint;

	float m_HitPoint_max;

	float GetHitPoint() {
		return m_HitPoint;
	}
	void SetHitPoint(float _hit_point) {
		m_HitPoint = max(0, min(m_HitPoint_max, _hit_point));
	}

	//�G�C���^�[�Q�b�g���ǂɓ������Ă��邩�ǂ���
	bool m_IsLookOn;
	//��t�b�N�����ʂ̖�ǂ���
	bool m_IsHook;

	float m_fov;

	int m_AimState;

	Base* mp_Arrow;

	//�t�b�N�̐��i���Ԃŉ񕜂���j
	int m_HookNum;
	//�t�b�N�̉񕜂��鎞��
	float m_HookCoolTime;
	//�t�b�N�̉񕜂��鎞��
	float m_HookRecoverTime;

	//���[�v��͂ޗp�̃X�t�B�A�R���C�_�[
	CSphere m_RopeShere;
	//���[�v��͂�ł��邩�ǂ���
	bool m_IsHangRope;
	//�W�����v�ł��邩�ǂ���
	bool m_IsJump;
	//�U�����x�X�e�[�^�X
	float m_attack_speed;
	//�ړ����x�X�e�[�^�X
	float m_move_speed;
	//�Ȃ񂱍U�����x�A�C�e���������Ă��邩
	int m_attack_speed_item_num;
	//���ړ����x�A�C�e���������Ă��邩
	int m_move_speed_item_num;

	float GetAttackSpeed() {
		return m_attack_speed;
	}
	void SetAttackSpeed(float _speed) {
		m_attack_speed = _speed;
	}
	void UpdateAttackSpeed(int attack_spped_item_num) {
		SetAttackSpeed(1.0f + (0.1f * attack_spped_item_num));
	}
	float GetMoveSpeed() {
		return m_move_speed;
	}
	void SetMoveSpeed(float _speed) {
		m_move_speed = _speed;
	}
	void UpdateMoveSpeed(int move_spped_item_num) {
		SetMoveSpeed(30.0f + (1.0f * move_spped_item_num));
	}
	int GetAttackSpeedItemNum() {
		return m_attack_speed_item_num;
	}
	void SetAttackSpeedItemNum(int _num) {
		m_attack_speed_item_num = max(0, _num);
	}
	int GetMoveSpeedItemNum() {
		return m_move_speed_item_num;
	}
	void SetMoveSpeedItemNum(int _num) {
		m_move_speed_item_num = max(0, _num);
	}
	

public:
	//�����l��v0��z��ɂ����
	std::vector<CVector3D> vecto;
public:
	Player(const Transform& _transform, const char* _modelName);
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
public:
	CVector3D GetCenterPoint();
	//void AddGravity();
	//void AddForce();
	void StandardRender();
	void HookRope(const CVector3D& _pos);
	void CollisionObject(CollisionTask* _task);
	void HangCheck(CollisionTask* _task);
	//void CollisionRope(CollisionTask* task);
	void DrawDebugInfo();
	void AttackCollision(CollisionTask* _task);
	void UpdateCollider();
	void CollisionCheckToField(CollisionTask* _task);
	void UpdateCamera()override;
	void HookRecover();
	void CollisionIsJump(CollisionTask* _task);
};