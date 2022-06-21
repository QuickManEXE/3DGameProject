#pragma once
#include "../../Base/Base.h"
#include"../../StateMachine/StateAI.h"
#include"../StaticMeshObject.h"



enum class FireElementalState {
	IdleState,//�A�C�h�����
	MoveState,//�U���͈͂Ɉړ�������
	AttackState,//�U�����
	DeathState,//�͐s����
};

class EnemyIdleState;
class EnemyMoveState;
class FireElementalAttackState;
class FireElementalDeathState;

class FireElemental : public Base {
public:
	//�t�����h�N���X�̐錾
	friend EnemyIdleState;
	friend EnemyMoveState;
	friend FireElementalAttackState;
	friend FireElementalDeathState;
public:
	//�G�̊e���ʂƂ̔���
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
	//���ʂ��Ƃ̃J�v�Z��
	static const int m_cupsule_max = 10;
	static Index m_culsule_idx[m_cupsule_max];
	CCapsule m_cupsule[m_cupsule_max];

	//�ړ��̓����蔻��
	float m_rad;

	int m_hp;

	enum AttackType {
		ThrowStone=3, //�Ⓤ��
		Strike=4,  //�O���Ԃ�Ȃ���
		SlamGround=5,    //�n�ʒ@���t��
	};

	int m_attack_type;

	//�U���͈̔�
	float m_attack_range;

	//�U�����Ă��邩���Ă��Ȃ���
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