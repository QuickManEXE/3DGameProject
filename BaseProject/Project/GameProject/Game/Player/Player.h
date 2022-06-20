#pragma once
#include "../CharacterBase/CharacterBase.h"
#include"../VariableParam.h"
#include"../../StateMachine/StateAI.h"
#include<memory>
#include"../Camera.h"

class WireRope;

enum class PlayerState {

	StayState,//操作不可能状態

	IdleState,//通常状態
	
	JumpState,//ジャンプ
	
	HangWallState,//掴み状態
	HangDropState,//掴み解除
	ClimeState,//壁上り

	WireAimState,//ワイヤーで狙うステート
	WireShotState,//ワイヤーを撃つ
	WireActionState,//ロープに捕まる
	
	ArrowAimState,//弓矢発射ステート
	
	DamageState,//ダメージ
	DeathState,//力尽きる
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
	//壁登り用の線コライダー(左右2つ)
	CLine m_LineCol[2];
	//ジャンプ用の線コライダー
	CLine m_JumpLine;
public:
	CVector3D m_OldPos;

	CVector3D m_ClimePoint;
	//掴んだロープのポインター
	WireRope* mp_WireRope;

	//ロ-プの接続点
	CVector3D m_RopeConectPoint;

	//カバ－しているときの背の面の法線
	CVector3D m_CoverWallNormal;

	VariableParam<float> m_Param;
	VariableParam<bool> m_Flag;

	int m_RandID;

	float m_HitPoint;

	//エイムターゲットが壁に当たっているかどうか
	bool m_IsLookOn;
	//矢がフックか普通の矢かどうか
	bool m_IsHook;

	float m_fov;

	int m_AimState;

	Base* mp_Arrow;

	//フックの数（時間で回復する）
	int m_HookNum;
	//フックの回復する時間
	float m_HookCoolTime;
	//フックの回復する時間
	float m_HookRecoverTime;

	//ロープを掴む用のスフィアコライダー
	CSphere m_RopeShere;
	//ロープを掴んでいるかどうか
	bool m_IsHangRope;
	//ジャンプできるかどうか
	bool m_IsJump;
public:
	//初期値のv0を配列にいれる
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