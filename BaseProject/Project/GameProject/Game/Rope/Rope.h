#pragma once
#include"../../Base/Base.h"
#include"../../StateMachine/StateAI.h"
#include"../Arrow.h"
#include"../Player/Player.h"


//ロープの基本クラス
class Rope : public Base {
private:

	CModelObj m_model;

	RenderTask m_Render;

	CollisionTask m_Col;

public:
	float rope_length;

	CVector3D lineS;
	CVector3D lineE;

	//親のロープポインター
	Rope* m_parent_rope;

	CMatrix m_top_matrix;
	CMatrix m_bottom_matrix;

	CCapsule m_capsule;


public:
	float ang_rope;

	float m_rad;
public:
	Rope(const Transform& transform,float length,Rope* rope = nullptr);
	void Update();
	void Render();
	void CollisionCheck(CollisionTask* _task);
};


class WireRope : public Base {
public:
	CModelObj m_model;
	CLine m_wire_line;
	Player* mp_player;
	Arrow* mp_arrow;
	RenderTask m_Render;
	CollisionTask m_Col;
public:
	WireRope(Player* mp_p,Arrow* mp_a);
	void Update()override;
	void Render();
	void SetLine(const CVector3D& start_pos, const CVector3D& end_pos);
};