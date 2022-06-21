#pragma once
#include "../CharacterBase/CharacterBase.h"
#include"../Dijkstra/Dijkstra.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticMeshObject.h"
#include"../Arrow.h"


enum class EnemyState {
	SearchState,//�T�����
	BattleIdleState,//�����
	IntervalState,//���[�g�T�����
	EnemyHangWallState,//�ǂ̂ڂ���
	AvoidState,//������
	ChaseState,//�ǐՏ��
	AttackState,//�U�����
	DamageState,//�_���[�W
	DeathState,//�͐s����
};


class Enemy : public CharacterBase {
public:
	//�{�[���̃C���f�b�N�X
	struct Index {
		int s;
		int e;
		float r;
	};

public:
	//�����蔻��p�R���C�_�[
	//�P���ʂ��Ƃ̃J�v�Z��
	static const int m_cupsule_max = 10;
	static Index m_culsule_idx[m_cupsule_max];
	CCapsule m_cupsule[m_cupsule_max];
	
	//�Ǔo��p�̐��R���C�_�[(���E2��)
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

	//�U�����Ă��邩�ǂ���
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
	
	//�����蔻��^���鑤
	void AttackCollide(CollisionTask* _task);

	//�����蔻��󂯂鑤
	void CollisionArrow(CollisionTask* _task);
	
};
