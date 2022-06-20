#pragma once

#include"../NonAnimObject.h"
#include"../../Singleton/Singleton.h"

//基本図形モデルオブジェクトクラス
class Goal : public NonAnimObject,public Singleton<Goal> {
public:
	friend class Singleton<Goal>;
private:

	CollisionTask m_Col;

	float m_rad;
public:
	Goal();
	void CollisionCheck(CollisionTask* _task);
};