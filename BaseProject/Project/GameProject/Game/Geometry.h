#pragma once
#include"NonAnimObject.h"
#include"../StarterAsset.h"

//��{�}�`���f���I�u�W�F�N�g�N���X
class Geometry : public NonAnimObject {
private:

	CollisionTask m_Col;

public:
	Geometry(const Transform& transform,const char* model_name = StarterAsset::Cube);
	//virtual void Render();
	void CollisionCheck(CollisionTask* _task);
};