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

	CModelObj m_door;

	Transform m_door_transform[max_count];

	CMatrix m_door_matrix[max_count];

	CollisionTask m_Col;

	float m_rad;
public:
	Entrance(int entrance_num);
	void Update()override;
	void Render()override;
	void CollisionCheck(CollisionTask* _task);
};