#pragma once
#include "../Base/Base.h"

class Arrow : public Base {
public:
	//モデルオブジェクト
	CModelObj m_model;
	CModel* mp_parent_model;
	CMatrix m_local_matrix;
	CMatrix m_world_matrix;
	int		m_parent_idx;
	bool	m_shot;
	RenderTask m_Render;
	CollisionTask m_Collision;
	//半径
	float m_rad;
	//カプセル用
	CVector3D m_s;
	CVector3D m_e;

	CVector3D* m_rope_pos;
public:
	//コンストラクタ
	Arrow();
	//更新
	void Update();
	//描画
	void Render();
	//当たり判定検証
	void CollisionCheck(CollisionTask* _task);
	void SetParent(CModel* p, int idx);
	void SetDir(const CVector3D& dir);
	void Shot(float pow, CVector3D* rope_pos=nullptr);
};