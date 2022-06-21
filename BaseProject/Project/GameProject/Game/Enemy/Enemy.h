#pragma once
#include "../CharacterBase/CharacterBase.h"
#include"../Dijkstra/Dijkstra.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticMeshObject.h"
#include"../Arrow.h"


enum class EnemyState {
	SearchState,//探す状態
	BattleIdleState,//交戦状態
	IntervalState,//ルート探索状態
	EnemyHangWallState,//壁のぼり状態
	AvoidState,//回避状態
	ChaseState,//追跡状態
	AttackState,//攻撃状態
	DamageState,//ダメージ
	DeathState,//力尽きる
};


class Enemy : public CharacterBase {
public:
	//ボーンのインデックス
	struct Index {
		int s;
		int e;
		float r;
	};

public:
	//当たり判定用コライダー
	//１部位ごとのカプセル
	static const int m_cupsule_max = 10;
	static Index m_culsule_idx[m_cupsule_max];
	CCapsule m_cupsule[m_cupsule_max];
	
	//壁登り用の線コライダー(左右2つ)
	CLine m_LineCol[2];
public:

	CollisionTask m_Col;
	CollisionTask m_AttackCol;

	RenderTask m_Render;

	StateAI<Enemy, EnemyState> m_StateAI;

	std::vector<Arrow*> m_ArrowVector;
public:
	Dijkstra m_Dijkstra;

	float m_rad;

	CVector3D m_ClimePoint;

	int m_HitPoint;

	//攻撃しているかどうか
	bool m_IsAttack;

	CVector3D m_PopPos;
public:
	Enemy(const Transform& _transform, const char* _modelName);
	virtual ~Enemy();
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
	
	void UpdateCollider();

	void DebugInfo();
	bool SearchPlayer();
	void HangCheck(CollisionTask* _task);
	void CanFallCheck(CollisionTask* _task);
	
	//当たり判定与える側
	void AttackCollide(CollisionTask* _task);

	//当たり判定受ける側
	void CollisionArrow(CollisionTask* _task);
	
};
