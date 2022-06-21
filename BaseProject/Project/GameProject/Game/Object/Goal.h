#pragma once

#include"../StaticMeshObject.h"
#include"../../Singleton/Singleton.h"

//��{�}�`���f���I�u�W�F�N�g�N���X
class Goal : public StaticMeshObject,public Singleton<Goal> {
public:
	friend class Singleton<Goal>;
private:

	CollisionTask m_Col;

	float m_rad;
public:
	Goal();
	void CollisionCheck(CollisionTask* _task);
};