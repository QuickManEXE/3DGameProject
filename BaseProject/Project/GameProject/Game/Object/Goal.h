#pragma once

#include"../StaticMeshObject.h"
#include"../../Singleton/Singleton.h"

//基本図形モデルオブジェクトクラス
class Goal : public StaticMeshObject,public Singleton<Goal> {
public:
	friend class Singleton<Goal>;
private:

	CollisionTask m_Col;

	float m_rad;

	float m_time;
	
	bool m_is_teleport;
public:
	Goal();
	void CollisionCheck(CollisionTask* _task);
	void Update()override;
	void SetIsTeleport(bool _is_teleport) {
		m_is_teleport = _is_teleport;
	}
	void SetTime(float _time) {
		m_time = _time;
	};
	void Teleport();
};