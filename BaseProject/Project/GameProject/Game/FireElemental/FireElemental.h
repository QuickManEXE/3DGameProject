#pragma once
#include "../../Base/Base.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticMeshObject.h"



enum class FireElementalState {
	IdleState,//アイドル状態
	MoveState,//攻撃範囲に移動する状態
	AttackState,//攻撃状態
	DeathState,//力尽きる
};

class EnemyIdleState;
class EnemyMoveState;
class FireElementalAttackState;
class FireElementalDeathState;

class FireElemental : public Base {
public:
	//フレンドクラスの宣言
	friend EnemyIdleState;
	friend EnemyMoveState;
	friend FireElementalAttackState;
	friend FireElementalDeathState;
public:
	//敵の各部位との判定
	struct Index {
		int s;
		int e;
		float r;
	};
private:

	CModelA3M m_model;

	CollisionTask m_col;

	CollisionTask m_attackCol;

	RenderTask m_render;

	StateAI<FireElemental, FireElementalState> m_stateAI;

private:
	//部位ごとのカプセル
	static const int m_cupsule_max = 10;
	static Index m_culsule_idx[m_cupsule_max];
	CCapsule m_cupsule[m_cupsule_max];

	//移動の当たり判定
	float m_rad;

	int m_hp;

	enum AttackType {
		ThrowStone=3, //岩投げ
		Strike=4,  //前方ぶんなぐり
		SlamGround=5,    //地面叩き付け
	};

	int m_attack_type;

	//攻撃の範囲
	float m_attack_range;

	//攻撃しているかしていないか
	bool m_is_attack;
public:
	FireElemental(const Transform& transform, const char* model_name);
	virtual ~FireElemental();
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
	void AddGravity();
	void StandardRender();
	bool SearchPlayer();
	
	void AttackCollide(CollisionTask* task);

	int SelectAttackType();

	void AddMoveForce();
	
	void GameObjectCollisionCheck(CollisionTask* task);

	void UpdateCollider();
	void CollisionArrow(CollisionTask* task);

	StateAI<FireElemental, FireElementalState> GetStateAI() {
		return m_stateAI;
	};
};