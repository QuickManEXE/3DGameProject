#pragma once

#include"../NonAnimObject.h"
#include"../../Singleton/Singleton.h"

//基本図形モデルオブジェクトクラス
class Entrance : public NonAnimObject{
public:
	static std::vector<Entrance*> m_entrances;
	
	int entrance_num;
private:
	

	CollisionTask m_Col;

	float m_rad;
public:
	Entrance(int entrance_num);
	void CollisionCheck(CollisionTask* _task);
};