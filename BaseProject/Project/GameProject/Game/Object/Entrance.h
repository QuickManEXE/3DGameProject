#pragma once

#include"../StaticMeshObject.h"
#include"../../Singleton/Singleton.h"

//基本図形モデルオブジェクトクラス
class Entrance : public StaticMeshObject{
public:
	static std::vector<Entrance*> m_entrances;
	
	int entrance_num;
private:

	enum {
		left_door,
		right_door,
		max_count,
	};

	bool m_is_open;//開くかどうか

	bool m_is_block;

	CModelObj m_door;

	Transform m_door_transform[max_count];

	CMatrix m_door_matrix[max_count];

	CollisionTask m_Col;

	float m_rad;


	CVector3D target_move;

	CVector3D target_pos;

	float time;

	CVector3D origin_pos;

	float m_block_rad;
public:
	Entrance(int entrance_num,const Transform& _transform);
	void Update()override;
	void Render()override;
	void CollisionCheck(CollisionTask* _task);
	void OpenEntrance();
	void SetIsBlock(bool _is_block) {
		m_is_block = _is_block;
	}
};