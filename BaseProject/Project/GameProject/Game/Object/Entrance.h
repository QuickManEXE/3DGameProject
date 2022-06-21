#pragma once

#include"../NonAnimObject.h"
#include"../../Singleton/Singleton.h"

//基本図形モデルオブジェクトクラス
class Entrance : public NonAnimObject{
public:
	static std::vector<Entrance*> m_entrances;
	
	int entrance_num;
private:
	bool m_is_open;//開くかどうか

	CModelObj m_door1;
	CModelObj m_door2;//素材

	Transform m_door1_transform;
	Transform m_door2_transform;

	CollisionTask m_Col;

	float m_rad;
public:
	Entrance(int entrance_num);
	void Update()override;
	void Render()override;
	void CollisionCheck(CollisionTask* _task);
};